// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerMovementState : uint8
{
	standing UMETA(DisplayName = "Standing"), walking UMETA(DisplayName = "Walking"), dashing UMETA(DisplayName = "Dashing")
};

UENUM()
enum class EPlayerActionState : uint8
{
	idle UMETA(DisplayName = "Idle"), attacking UMETA(DisplayName = "Attacking"), carrying UMETA(DisplayName = "Carrying")
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
	class UInputAction* AttackAction;

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
	EPlayerActionState CurrentActionState;

	void SetPlayerActionState(EPlayerActionState NewActionState);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float throwVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AWeapon> DefaultWeapon;

	AWeapon* WeaponOnGround;


	//DASH MECHANICS
	UPROPERTY(EditAnywhere, Category = Dash)
	float DashImpulseStrength;
	UPROPERTY(EditAnywhere, Category = Dash)
	float DashCooldown;

	bool bCanDash;

	FTimerHandle DashCooldownTimerHandle;

	void ResetDash();

	UCapsuleComponent* CollisionCapsule;


	//AI Perception
	UPROPERTY(EditDefaultsOnly)
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DoMove(const FInputActionValue& Value);

	void DoLook(const FInputActionValue& Value);

	void DoDash(const FInputActionValue& Value);

	void StartAttack(const FInputActionValue& Value);

	void StopAttack(const FInputActionValue& Value);

	void InteractPressed(const FInputActionValue& Value);

	void InteractReleased(const FInputActionValue& Value);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsCarryingItem();
	bool IsAttacking();
	bool IsFacingWall();

	UFUNCTION(Server, Unreliable)
	void Server_DropCarriedItem();

	UFUNCTION(Server, Unreliable)
	void Server_PickupItem(class ACarryableActor* itemToCarry);

	UFUNCTION(Server, Reliable)
	void Server_OnInteract(bool interacting);

	UFUNCTION(Server, Unreliable)
	void Server_DoDash(FVector Impulse);

	UFUNCTION(Server, Unreliable)
	void Server_EquipWeapon(AWeapon* Weapon);

	UFUNCTION(Server, Unreliable)
	void Server_SpawnDefaultWeapon();

};
