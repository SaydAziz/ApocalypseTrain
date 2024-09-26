// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/InteractableActor.h"
#include "Core/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/ATGameMode.h"
#include "Core/ATGameState.h"
#include "Core/PlayerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	playerManager = GetWorld()->GetGameState<AATGameState>()->GetPlayerManager();
	trigger = FindComponentByTag<USphereComponent>(FName("Trigger"));
	MeshToHighlight = FindComponentByTag<UStaticMeshComponent>(FName("MeshToHighlight"));
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, FString::Printf(TEXT("Players: %d"), overlappingPlayers.Num()));
	if (ShouldUpdateOverlappingPlayers()) {
		UpdateOverlappingPlayers();
	}
}
 
//send Interacted to server
//send OnInteract to clients

//onplayer overlap for player - on ground item = onoverlap on overlap exit, set to null 
//on interact - call interact on the ongrounditem 



bool AInteractableActor::ShouldUpdateOverlappingPlayers()
{
	return true;
}

void AInteractableActor::UpdateOverlappingPlayers()
{
	if (playerManager == NULL) {
		return;
	}
	for (APlayerCharacter* c : playerManager->ActivePlayers) {
		if (IsOverlappingActor(c) && !overlappingPlayers.Contains(c) && CanPlayerInteract(c)) {
			overlappingPlayers.Add(c);
			Multi_PlayersOverlapping();
		}
	}
	if (CurrentlyInteractable()) {
		if (overlappingPlayers.Num() > 0) {
			CheckForInteractPressed();
		}
	}
	for (APlayerCharacter* c : playerManager->ActivePlayers) {
		if (!IsOverlappingActor(c) && overlappingPlayers.Contains(c)) {
			overlappingPlayers.Remove(c);
			if (overlappingPlayers.Num() <= 0) {
				Multi_NoPlayersOverlapping();
			}
		}
	}
}

void AInteractableActor::CheckForInteractPressed()
{
	if (wasInteracted) {
		return;
	}
	for (APlayerCharacter* player : overlappingPlayers) {
		if (player->Interacted) {
			OnInteract(player);
		}
	}
}

void AInteractableActor::OnInteract(APlayerCharacter* player)
{
	OnInteracted();
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString(TEXT("INTERACTING WITH YOUR MOM")));
}

bool AInteractableActor::CurrentlyInteractable()
{
	return true;
}

bool AInteractableActor::CanPlayerInteract(APlayerCharacter* player)
{
	return true;
}

void AInteractableActor::Multi_PlayersOverlapping_Implementation()
{
	if (MeshToHighlight != NULL && MeshToHighlight->bRenderCustomDepth == false) {
		MeshToHighlight->SetRenderCustomDepth(true);
	}
}

void AInteractableActor::Multi_NoPlayersOverlapping_Implementation()
{
	if (MeshToHighlight != NULL && MeshToHighlight->bRenderCustomDepth == true) {
		MeshToHighlight->SetRenderCustomDepth(false);
	}
}

void AInteractableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractableActor, overlappingPlayers)
	DOREPLIFETIME(AInteractableActor, overlappingPlayersToRemove)
}
