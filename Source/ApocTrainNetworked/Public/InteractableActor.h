// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class APlayerCharacter;

UCLASS()
class APOCTRAINNETWORKED_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AInteractableActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	TArray<APlayerCharacter*> overlappingPlayers;
	UPROPERTY(Replicated)
	TArray<APlayerCharacter*> overlappingPlayersToRemove;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class USphereComponent* trigger;

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayersOverlapping();
	void Multi_PlayersOverlapping_Implementation();
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_NoPlayersOverlapping();
	void Multi_NoPlayersOverlapping_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnInteract(APlayerCharacter* player);

protected:

	bool wasInteracted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshToHighlight;

	class APlayerManager* playerManager;

	void UpdateOverlappingPlayers();

	virtual void CheckForInteractPressed();

	virtual bool CurrentlyInteractable();

	virtual bool CanPlayerInteract(APlayerCharacter* player);

};
