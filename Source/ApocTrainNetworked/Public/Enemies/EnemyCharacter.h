// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/EnemyData.h"
#include "Systems/Poolable.h"
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
class APOCTRAINNETWORKED_API AEnemyCharacter : public ACharacter, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:

	bool bIsDead;
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

	virtual void InitializeEnemy();

	bool bCanAttack;

	AEnemyAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Cosmetic")
	UNiagaraComponent* BloodSplatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cosmetic")
	UNiagaraSystem* BloodSplatSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDamageComponent* DamageComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawning)
	bool bIsNotPooled;

	UBehaviorTree* GetBehaviorTree() const;

	UFUNCTION()
	virtual void TakeDamage(float damageToTake);

	// Inherited via IPoolable
	void OnSpawn(FVector SpawnLocation) override;

	UFUNCTION()
	void OnDespawn() override;

	bool CanSpawn() override;

	void IncreaseEnemyDifficulty();

	//ATTACK

	float GetAttackRadius();

	void ExecuteMeleeAttack();

	bool CanAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void EnableAttackBox();

	void DisableAttackBox();


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

	//ON DAMAGE
	UFUNCTION(Server, Unreliable)
	void Server_OnDamaged(float damageTaken);
	void Server_OnDamaged_Implementation(float damageTaken);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_OnDamaged(float damageTaken);
	void Multi_OnDamaged_Implementation(float damageTaken);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float health);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
