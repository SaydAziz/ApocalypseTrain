// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AEnemyAIController(FObjectInitializer const& ObjectInitializer);

	void SetIsDead(bool isDead);

	void SetSightSenseValues(float SightRadius);

protected:
	virtual void OnPossess(APawn* InPawn) override;

	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

};
