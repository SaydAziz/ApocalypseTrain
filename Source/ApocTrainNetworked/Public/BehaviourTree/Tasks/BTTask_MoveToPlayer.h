// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API UBTTask_MoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void MoveToPlayer(APawn* EnemyPawn, APawn* PlayerPawn);
};
