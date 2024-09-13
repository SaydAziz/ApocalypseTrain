// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnManager.h"
#include "CustomUtils.h"
#include "ChunkSpawner.h"
#include "ObjectPooler.h"
#include <ATGameState.h>
#include "EnemyData.h"

// Sets default values
AEnemySpawnManager::AEnemySpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	ResetEnemyData();
	GetWorld()->GetGameState<AATGameState>()->OnSpawnEnemies.AddDynamic(this, &AEnemySpawnManager::SpawnEnemiesOnChunk);
	GetWorld()->GetGameState<AATGameState>()->OnDifficultyIncrease.AddDynamic(this, &AEnemySpawnManager::IncreaseDifficulty);
	CreateEnemyPools();
}

void AEnemySpawnManager::SpawnEnemiesOnChunk(float Ypos)
{
	if (HasAuthority()) {
		for (AObjectPooler* p : EnemyPools) {
			for (int i = 0; i < EnemiesPerChunk; i++) {
				p->SpawnObject(FVector(i * 50, Ypos, 40));
			}
		}
	}
}

void AEnemySpawnManager::IncreaseDifficulty()
{
	for (UEnemyData* data : EnemyDataList) {
		data->IncreaseDifficulty();
	}
}

void AEnemySpawnManager::CreateEnemyPools()
{
	if (HasAuthority()) {
		for (FEnemySpawnData e : SpawnData) {
			AObjectPooler* p = GetWorld()->SpawnActor<AObjectPooler>();
			p->InitializePoolerFromSpawn(e.enemyType, e.TotalEnemies);
			EnemyPools.Add(p);
		}
	}
}

void AEnemySpawnManager::ResetEnemyData()
{
	for (UEnemyData* e : EnemyDataList) {
		e->ResetValues();
	}
}

// Called every frame
void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

