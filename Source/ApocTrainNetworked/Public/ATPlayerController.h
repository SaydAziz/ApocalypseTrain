// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AATPlayerController : public APlayerController
{
	
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	int PlayerIndex;

	int LocalPlayerIndex;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetLocalPlayerIndex();

};
