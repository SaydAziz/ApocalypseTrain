// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnedChunkInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Location")
	float YPos;

};

class ALevelChunk;
class AEncounterChunk;

DECLARE_DELEGATE_OneParam(FChunkSpawned, FSpawnedChunkInfo);

UCLASS()
class APOCTRAINNETWORKED_API AChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkSpawner();

protected:

	class AATGameState* gameState;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AActor* leadingActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float targetPostion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float leaderTargetOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float navMeshOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category =spawning)
	int EncounterSpawnFrequency;

	void SpawnNewChunk();

	int prevChunkIndex;
	int prevEncounterChunkIndex;

	TSubclassOf<ALevelChunk> GetRegularChunkType();
	TSubclassOf<AEncounterChunk> GetEncounterChunkType();

	bool TargetReached();

	void SetLeadingActor(AActor* actor);

	class ANavMeshBoundsVolume* NavMeshBounds;

	void RelocateNavMesh(FVector newLocation);

	int TotalChunksSpawned;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class ALevelChunk> > LevelChunks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AEncounterChunk> > EncounterChunks;
	
	FChunkSpawned OnChunkSpawned;
};
