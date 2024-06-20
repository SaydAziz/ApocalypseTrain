// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ATGameState.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AATGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerManager* GetPlayerManager();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerManager* playerManager;
};
