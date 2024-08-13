// Fill out your copyright notice in the Description page of Project Settings.


#include "EncounterChunk.h"
#include "CustomUtils.h"
#include "Train.h"
#include "ATGameState.h"

void AEncounterChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!hasActivated) {
		if (train->GetActorLocation().Y >= GetActorLocation().Y - TriggerOffset) {
			GetWorld()->GetGameState<AATGameState>()->SetGameState(EGameState::Encounter);
			hasActivated = true;
		}
	}
}

void AEncounterChunk::BeginPlay()
{
	Super::BeginPlay();
	train = CustomUtils::GetFirstActorOfClass<ATrain>(GetWorld());
}
