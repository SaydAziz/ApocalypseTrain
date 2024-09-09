// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPlayerInHitRange.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API UBTService_CheckPlayerInHitRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPlayerInHitRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="AI")
	float Range = 10.0f;

	UPROPERTY(EditAnywhere, Category="AI")
	FName PlayerInHitRangeKey = "PlayerInHitRange";
};
