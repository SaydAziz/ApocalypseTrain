// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

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

}

// Called every frame
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

int APlayerManager::RegisterPlayerWithManager(APlayerCharacter* joinedPlayer)
{
	int index = ActivePlayers.Add(joinedPlayer);
	joinedPlayer->PlayerIndex = index;
	return index;
}

