// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"



UCLASS()
class APOCTRAINNETWORKED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();



	//INPUT CONTROLS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

	//INTERACTION MECHANICS
	class ACarryableActor* carriedObject;
	class USceneComponent* carrySlot;
	class USkeletalMeshComponent* characterMesh;

//protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool Interacted;

	UPROPERTY(BlueprintReadOnly)
	int PlayerIndex;

protected:

	//INTERACT MECHANICS
	UPROPERTY(BlueprintReadOnly)
	bool CarryingItem;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float throwVelocity;

	//DASH MECHANICS
	UPROPERTY(EditAnywhere, Category = Dash)
	float DashImpulseStrength;
	UPROPERTY(EditAnywhere, Category = Dash)
	float DashCooldown;

	bool bCanDash;
	FTimerHandle DashCooldownTimerHandle;

	void ResetDash();


	class USphereComponent* InteractionTrigger;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DoMove(const FInputActionValue& Value);

	void DoLook(const FInputActionValue& Value);

	void DoDash(const FInputActionValue& Value);

	void InteractPressed(const FInputActionValue& Value);

	void InteractReleased(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsCarryingItem();
	bool IsFacingWall();

	UFUNCTION(Server, Unreliable)
	void Server_DropCarriedItem();
	void Server_DropCarriedItem_Implementation();

	UFUNCTION(Server, Unreliable)
	void Server_PickupItem(class ACarryableActor* itemToCarry);
	void Server_PickupItem_Implementation(class ACarryableActor* itemToCarry);

	UFUNCTION(Server, Reliable)
	void Server_OnInteract(bool interacting);
	void Server_OnInteract_Implementation(bool interacting);

};
