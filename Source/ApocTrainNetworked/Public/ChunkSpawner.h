// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkSpawner.generated.h"


UCLASS()
class APOCTRAINNETWORKED_API AChunkSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AActor* leadingActor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float targetPostion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float leaderTargetOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float navMeshOffset;

	void SpawnNewChunk();

	bool TargetReached();

	void SetLeadingActor(AActor* actor);

	class ANavMeshBoundsVolume* NavMeshBounds;

	void RelocateNavMesh(FVector newLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class ALevelChunk> > LevelChunks;
	
};
