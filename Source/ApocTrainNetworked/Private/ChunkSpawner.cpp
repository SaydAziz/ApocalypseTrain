// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkSpawner.h"
#include "LevelChunk.h"
#include "CustomUtils.h"
#include <Train.h>

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
}

void AChunkSpawner::SpawnNewChunk()
{
	int chunkIndex = FMath::RandRange(0,LevelChunks.Num()-1);
	ALevelChunk* chunk = Cast<ALevelChunk>(GetWorld()->SpawnActor(LevelChunks[chunkIndex],new FVector(0, targetPostion, -40), new FRotator(), FActorSpawnParameters()));
	targetPostion += chunk->GetChunkLength();
	GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Blue, FString::Printf(TEXT("new target position: %f"), targetPostion));
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

