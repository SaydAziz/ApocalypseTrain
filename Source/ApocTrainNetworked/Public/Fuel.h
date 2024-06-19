// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarryableActor.h"
#include "Fuel.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AFuel : public ACarryableActor
{
	GENERATED_BODY()
	

protected:

	UFUNCTION(BlueprintCallable)
	void OnFuelAddedToTrain();
};
