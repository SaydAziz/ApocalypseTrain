// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class APOCTRAINNETWORKED_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	float SightRadius;

	void IncreaseDifficulty();
	
};


//enemeis subscribe to level up event chunk spawning after certain number send out level up event, go through GameState