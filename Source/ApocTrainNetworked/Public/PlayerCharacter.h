// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Damagable.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	standing UMETA(DisplayName = "Standing"), walking UMETA(DisplayName = "Walking"), dashing UMETA(DisplayName = "Dashing")
};

UENUM(BlueprintType)
enum class EPlayerActionState : uint8
{
	idle UMETA(DisplayName = "Idle"), attacking UMETA(DisplayName = "Attacking"), carrying UMETA(DisplayName = "Carrying")
};


UCLASS()
class APOCTRAINNETWORKED_API APlayerCharacter : public ACharacter, public IDamagable
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
	UPROPERTY(Replicated, BlueprintReadOnly)
	int PlayerIndex;

	int GetPlayerIndex();
	//void SetPlayerIndex(int index);

protected:

	class APlayerManager* playerManager;

	//PLAYER STATE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerMovementState CurrentMovementState;

	void SetPlayerMovementState(EPlayerMovementState NewMovementState);
	UFUNCTION(BlueprintCallable, Category="Movement")
	EPlayerMovementState GetPlayerMovementState() const;

	//INTERACT MECHANICS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerActionState CurrentActionState;

	void SetPlayerActionState(EPlayerActionState NewActionState);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interact)
	float throwVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AWeapon> DefaultWeapon;

	AWeapon* WeaponOnGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float CurrentHealth;


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

	//CONTROLS
	UPROPERTY(Replicated)
	bool bIsUsingMouse;

	void DoMove(const FInputActionValue& Value);

	void DoLook(const FInputActionValue& Value);

	void DoDash(const FInputActionValue& Value);

	void StartAction(const FInputActionValue& Value);

	void StopAction(const FInputActionValue& Value);

	void StopAttacking();

	void InteractPressed(const FInputActionValue& Value);

	void InteractReleased(const FInputActionValue& Value);

	FVector GetHitResultUnderCursor();

	void RotateCharacterToLookAt(const FVector TargetPosition);

	//SPAWNING
	bool bIsDead;

	void DespawnPlayer();
	void RespawnPlayer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Respawn)
	int respawnTime;
	int currentRespawnTime;
	FTimerHandle respawnTimerHandle;

	//DAMAGE
	FTimerHandle damageSlowTimerHandle;
	void ResetMovementSpeed();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (ToolTip = "How long the player is slowed down for after taking damage"))
	float DamageSlowTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = health)
	float MaxHealth;

	//MOVEMENT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float InjuredMoveSpeed;

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

	void OnPlayerRegistered(class APlayerManager* manager);

	bool IsCarryingItem();
	bool IsAttacking();
	bool IsFacingWall();

	virtual void Damage(float damageToTake);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stats")
	virtual float GetHealth();

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
