// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelChunk.h"
#include "EncounterChunk.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AEncounterChunk : public ALevelChunk
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TriggerOffset;

protected:
	virtual void BeginPlay() override;

	class ATrain* train;

	bool hasActivated;
};
