// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ATGameState.h"
#include "Core/PlayerManager.h"
#include "LevelGen/ChunkSpawner.h"
#include "Core/PlayerCharacter.h"
#include "Enemies/EnemyData.h"

APlayerManager* AATGameState::GetPlayerManager()
{
	return playerManager;
}

void AATGameState::OnChunkSpawned(FSpawnedChunkInfo info)
{
	OnSpawnEnemies.Broadcast(info.YPos);
}

void AATGameState::SetGameState(EGameState stateToSet)
{
	switch (stateToSet) {
		case EGameState::Encounter:
			if (CurrentGameState == EGameState::traveling) {
				OnDifficultyIncrease.Broadcast();
			}
			break;
		case EGameState::lobby:
			break;
		case EGameState::traveling:
			break;
	}
	CurrentGameState = stateToSet;
}

EGameState AATGameState::GetGameState()
{
	return CurrentGameState;
}





