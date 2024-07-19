// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyData.h"
#include "Damagable.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class APOCTRAINNETWORKED_API AEnemyCharacter : public ACharacter, public IDamagable
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

	float currentHealth;

	virtual void InitializeEnemy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	virtual void Damage(float damageToTake);

	virtual float GetHealth();


};
