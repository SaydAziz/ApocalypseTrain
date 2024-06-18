// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "CarryableActor.generated.h"

/**
 * 
 */

UENUM()
enum class ECarriedState : uint8 {
	Idle UMETA(DisplayName = "Idle"), Carried UMETA(DisplayName = "Carried"), Dropped UMETA(DisplayName = "Dropped")
};

UCLASS()
class APOCTRAINNETWORKED_API ACarryableActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:

	ECarriedState carriedState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerCharacter* LastCarryingPlayer;

	void DropObject(FVector directionToLaunch);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickedUp(USceneComponent* carrier);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDropped(FVector directionToLaunch);

	virtual bool CurrentlyInteractable() override;

};
