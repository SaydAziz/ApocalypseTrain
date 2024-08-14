// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkSpawner.h"
#include "EncounterChunk.h"
#include "CustomUtils.h"
#include <Train.h>
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include <ATGameState.h>

// Sets default values
AChunkSpawner::AChunkSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChunkSpawner::BeginPlay()
{
	Super::BeginPlay();
	SetLeadingActor(CustomUtils::GetFirstActorOfClass<ATrain>(GetWorld()));
	NavMeshBounds = CustomUtils::GetFirstActorOfClass<ANavMeshBoundsVolume>(GetWorld());
	gameState = GetWorld()->GetGameState<AATGameState>();
	OnChunkSpawned.BindUObject(gameState, &AATGameState::OnChunkSpawned);
}

void AChunkSpawner::SpawnNewChunk()
{
	if (HasAuthority()) {
		ALevelChunk* chunk;
		if (TotalChunksSpawned <= 1 || TotalChunksSpawned % EncounterSpawnFrequency != 0) {
			chunk = Cast<ALevelChunk>(GetWorld()->SpawnActor(GetRegularChunkType(), new FVector(0, targetPostion, -40), new FRotator(), FActorSpawnParameters()));
		}
		else {
			chunk = Cast<AEncounterChunk>(GetWorld()->SpawnActor(GetEncounterChunkType(), new FVector(0, targetPostion, -40), new FRotator(), FActorSpawnParameters()));
		}
		RelocateNavMesh(FVector(0, targetPostion - navMeshOffset, 0));
		FSpawnedChunkInfo info = FSpawnedChunkInfo();
		info.YPos = targetPostion;
		OnChunkSpawned.ExecuteIfBound(info);
		targetPostion += chunk->GetChunkLength();
		TotalChunksSpawned++;
	}
}


TSubclassOf<ALevelChunk> AChunkSpawner::GetRegularChunkType()
{
	int chunkIndex = FMath::RandRange(0, LevelChunks.Num() - 1);
	while (chunkIndex == prevChunkIndex) {
		chunkIndex = FMath::RandRange(0, LevelChunks.Num() - 1);
	}
	prevChunkIndex = chunkIndex;
	return LevelChunks[chunkIndex];
}

TSubclassOf<AEncounterChunk> AChunkSpawner::GetEncounterChunkType()
{
	int chunkIndex = FMath::RandRange(0, EncounterChunks.Num() - 1);
	while (EncounterChunks.Num() > 1 && chunkIndex == prevEncounterChunkIndex) {
		chunkIndex = FMath::RandRange(0, EncounterChunks.Num() - 1);
	}
	prevEncounterChunkIndex = chunkIndex;
	return EncounterChunks[chunkIndex];
}

bool AChunkSpawner::TargetReached()
{
	if (leadingActor && leadingActor->GetActorLocation().Y >= targetPostion - leaderTargetOffset) {
		return true;
	}
	return false;
}

// Called every frame
void AChunkSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetReached()) {
		SpawnNewChunk();
	}
}

void AChunkSpawner::SetLeadingActor(AActor* actor)
{
	leadingActor = actor;
}

void AChunkSpawner::RelocateNavMesh(FVector newLocation)
{
	if (NavMeshBounds) {
		NavMeshBounds->SetActorLocation(newLocation);
		if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())) {
			NavSys->OnNavigationBoundsUpdated(NavMeshBounds);
		}
	}
}

