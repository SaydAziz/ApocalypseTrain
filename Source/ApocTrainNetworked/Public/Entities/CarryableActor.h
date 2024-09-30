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

	ACarryableActor();

	virtual void BeginPlay() override;

	ECarriedState carriedState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerCharacter* LastCarryingPlayer;
	
	UPrimitiveComponent* PhysicsComponent;

	virtual void OnInteract(APlayerCharacter* player) override;


	virtual bool CurrentlyInteractable() override;

	bool IsCarried();

	UFUNCTION()
	void OnPhysicsComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION(Server, Unreliable)
	void Server_OnPickedUp(USkeletalMeshComponent* carrier);
	void Server_OnPickedUp_Implementation(USkeletalMeshComponent* carrier);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_SetRenderDepth(bool renderdepth);
	void Multi_SetRenderDepth_Implementation(bool renderdepth);

	UFUNCTION(Server, Reliable)
	virtual void Server_DropObject(FVector directionToLaunch, FVector dropperLocation);
	virtual void Server_DropObject_Implementation(FVector directionToLaunch, FVector dropperLocation);


protected:

	UPROPERTY(BlueprintReadOnly);
	bool wasDropped;

	UPROPERTY(EditDefaultsOnly)
	class UPulseComponent* pulseComponent;

	virtual bool ShouldUpdateOverlappingPlayers() override;
};
