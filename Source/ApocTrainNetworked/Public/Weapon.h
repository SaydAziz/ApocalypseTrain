// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponData.h"
#include "Weapon.generated.h"


UCLASS(Abstract)
class APOCTRAINNETWORKED_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UWeaponData* Data;

	FTimerHandle AttackRateTimerHandle;
	
	virtual void Equip();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartAttack();
	virtual void Attack();
	virtual void StopAttack();

};
