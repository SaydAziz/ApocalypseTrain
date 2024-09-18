// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyData.h"
#include "Damagable.h"
#include "Poolable.h"
#include "Components/BoxComponent.h"
#include "EnemyCharacter.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AEnemyAIController;

UENUM(BlueprintType)
enum class EEnemyState: uint8
{
	idle UMETA(DisplayName = "Idle"), chasing UMETA(DisplayName = "Chasing"), attacking UMETA(DisplayName = "Attacking")
};

UCLASS()
class APOCTRAINNETWORKED_API AEnemyCharacter : public ACharacter, public IDamagable, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess=true))
	UBehaviorTree* Tree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFlashComponent* FlashComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category=Enemy)
	UEnemyData* EnemyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
	UBoxComponent* AttackBox;

	float currentHealth;

	virtual void InitializeEnemy();

	//temp bool for isdead, should probobly make this a state
	bool bIsDead;

	bool bCanAttack;

	AEnemyAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Cosmetic")
	UNiagaraComponent* BloodSplatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic")
	UNiagaraSystem* BloodSplatSystem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawning)
	bool bIsNotPooled;

	UBehaviorTree* GetBehaviorTree() const;

	virtual void Damage(float damageToTake);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Stats")
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	virtual float GetMaxHealth();

	// Inherited via IPoolable
	void OnSpawn(FVector SpawnLocation) override;

	void OnDespawn() override;

	bool CanSpawn() override;

	void IncreaseEnemyDifficulty();

	float GetAttackRadius();

	void ExecuteMeleeAttack();

	bool CanAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void EnableAttackBox();

	void DisableAttackBox();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle AttackRateTimerHandle;
	FTimerHandle AttackBoxTimerHandle;

	void ResetAttack();

	//PLAYER STATE
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	EEnemyState CurrentState;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void SetEnemyState(EEnemyState NewState);

	UFUNCTION(BlueprintCallable, Category="Movement")
	EEnemyState GetEnemyState() const;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
