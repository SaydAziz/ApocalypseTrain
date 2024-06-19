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

	virtual void BeginPlay() override;

	ECarriedState carriedState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerCharacter* LastCarryingPlayer;
	
	UPrimitiveComponent* PhysicsComponent;

	virtual void OnInteract(APlayerCharacter* player) override;


	virtual bool CurrentlyInteractable() override;


	UFUNCTION()
	void OnPhysicsComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION(Server, Unreliable)
	void Server_OnPickedUp(USceneComponent* carrier);
	void Server_OnPickedUp_Implementation(USceneComponent* carrier);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnPickedUp(USceneComponent* carrier);
	void Multi_OnPickedUp_Implementation(USceneComponent* carrier);

	UFUNCTION(Server, Unreliable)
	void Server_DropObject(FVector directionToLaunch);
	void Server_DropObject_Implementation(FVector directionToLaunch);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_DropObject(FVector directionToLaunch);
	void Multi_DropObject_Implementation(FVector directionToLaunch);

protected:

	UPROPERTY(BlueprintReadOnly);
	bool wasDropped;


};
