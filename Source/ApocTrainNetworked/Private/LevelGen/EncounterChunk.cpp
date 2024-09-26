// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGen/EncounterChunk.h"
#include "Systems/CustomUtils.h"
#include "Core/Train.h"
#include "Core/ATGameState.h"

void AEncounterChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!hasActivated) {
		if (train->GetActorLocation().Y >= GetActorLocation().Y - TriggerOffset) {
			GetWorld()->GetGameState<AATGameState>()->SetGameState(EGameState::Encounter);
			hasActivated = true;
			SetActorTickEnabled(false);
		}
	}
}

void AEncounterChunk::BeginPlay()
{
	Super::BeginPlay();
	train = CustomUtils::GetFirstActorOfClass<ATrain>(GetWorld());
}
