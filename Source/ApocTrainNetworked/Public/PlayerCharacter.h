// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerMovementState : uint8
{
	standing UMETA(DisplayName = "Standing"), walking UMETA(DisplayName = "Walking"), dashing UMETA(DisplayName = "Dashing")
};

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

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool Interacted;
	UPROPERTY(BlueprintReadOnly)
	int PlayerIndex;


protected:

	//PLAYER STATE
	EPlayerMovementState CurrentMovementState;

	void SetPlayerMovementState(EPlayerMovementState NewMovementState);

	//INTERACT MECHANICS
	UPROPERTY(BlueprintReadOnly)
	bool CarryingItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float throwVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AWeapon> DefaultWeapon;


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

	void EquipWeapon(AWeapon* Weapon);
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
