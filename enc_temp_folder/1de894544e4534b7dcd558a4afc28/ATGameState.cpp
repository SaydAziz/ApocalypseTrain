// Fill out your copyright notice in the Description page of Project Settings.


#include "ATGameState.h"
#include "PlayerManager.h"
#include "ChunkSpawner.h"
#include "PlayerCharacter.h"
#include "EnemyData.h"

APlayerManager* AATGameState::GetPlayerManager()
{
	return playerManager;
}

void AATGameState::OnChunkSpawned(FSpawnedChunkInfo info)
{
	OnSpawnEnemies.Broadcast(info.YPos);
	OnDifficultyIncrease.Broadcast();
}





