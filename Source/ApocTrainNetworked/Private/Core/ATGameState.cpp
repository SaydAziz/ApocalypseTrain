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
			break;
		case EGameState::lobby:
			break;
		case EGameState::traveling:
			if (CurrentGameState == EGameState::Encounter) {
				OnDifficultyIncrease.Broadcast();
			}
			break;
	}
	CurrentGameState = stateToSet;
}

EGameState AATGameState::GetGameState()
{
	return CurrentGameState;
}





