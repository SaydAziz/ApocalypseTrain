// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PlayerManager.h"
#include "Core/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Systems/CustomUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/Train.h"

// Sets default values
APlayerManager::APlayerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void APlayerManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void APlayerManager::BeginPlay()
{
	Super::BeginPlay();
	train = CustomUtils::GetFirstActorOfClass<ATrain>(GetWorld());
}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

FVector APlayerManager::GetPlayerDeathPos()
{
	return playerDeathPosition;
}

int APlayerManager::RegisterPlayerWithManager(APlayerCharacter* joinedPlayer)
{
	int index = ActivePlayers.Add(joinedPlayer);
	joinedPlayer->PlayerIndex = index;
	joinedPlayer->OnPlayerRegistered(this);
	return index;
}

bool APlayerManager::IsOverlappingPlayer(UBoxComponent* box)
{
	for (int i = 0; i < ActivePlayers.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(ActivePlayers[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			return true;
		}
	}
	return false;
}

bool APlayerManager::IsOverlappingPlayerWithFuel(UBoxComponent* box)
{
	for (int i = 0; i < ActivePlayers.Num(); i++) {
		if (UKismetMathLibrary::IsPointInBox(ActivePlayers[i]->GetActorLocation(), box->GetComponentLocation(), box->GetScaledBoxExtent()))
		{
			if (ActivePlayers[i]->IsCarryingItem()) {
				return true;
			}
		}
	}
	return false;
}

APlayerCharacter* APlayerManager::GetClosestPlayer(FVector location)
{
	APlayerCharacter* closestPlayer = nullptr;
	if (ActivePlayers.Num() > 0) {

		closestPlayer = ActivePlayers[0];
		for (APlayerCharacter* p : ActivePlayers) {
			if (FVector::Dist(p->GetActorLocation(), location) < FVector::Dist(closestPlayer->GetActorLocation(), location)) {
				closestPlayer = p;
			}
		}
	}
	return closestPlayer;
}

FVector APlayerManager::GetPlayerSpawnPoint(int PlayerIndex)
{
	//this is super messy and too many magic numbers. Should expose these offset numbers to BP with good name conventions
	FVector pos = SpawnOffset; 
	float x = -(PlayerIndex * 100);
	pos.Y -= 400;
	if (PlayerIndex % 2 == 0) {
		pos.Y -= 200;
	}
	pos.Y -= 300;
	pos.X += x;
	pos.X -= 100;
	pos.Z += 100;
	if(train)
	{
		pos.Y += train->GetActorLocation().Y;
	}
	return pos;
}

bool APlayerManager::IsOutOfBounds(FVector location)
{
	if (location.Z < -100) {
		return true;
	}
	return false;
}


