// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DoMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API UBTTask_DoMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_DoMeleeAttack(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
