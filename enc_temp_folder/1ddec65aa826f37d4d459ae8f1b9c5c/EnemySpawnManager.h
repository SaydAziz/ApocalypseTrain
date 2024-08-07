// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> enemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalEnemies;
};

UCLASS()
class APOCTRAINNETWORKED_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnEnemiesOnChunk(float Ypos);
	UFUNCTION()
	void IncreaseDifficulty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnData> SpawnData;

	TArray<class AObjectPooler*> EnemyPools;

	TArray<class AEnemyCharacter*> AllEnemies;

	void CreateEnemyPools();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemiesPerChunk;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyData)
	TArray<class UEnemyData*> EnemyDataList;
};

