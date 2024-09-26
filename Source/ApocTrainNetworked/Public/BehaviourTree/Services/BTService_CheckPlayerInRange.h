// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API UBTService_CheckPlayerInRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayerInRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="AI")
	float Range = 1000.0f;

	UPROPERTY(EditAnywhere, Category="AI")
	FName PlayerInRangeKey = "PlayerInRange";
};
