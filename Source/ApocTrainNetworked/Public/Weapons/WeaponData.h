// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class APOCTRAINNETWORKED_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Penetration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float RandomShotConeRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int ShotsPerAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	UNiagaraSystem* BulletTracer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX)
	UNiagaraSystem* MuzzleFlash;
};
