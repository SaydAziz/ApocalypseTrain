// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToRandomLoc.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API UBTTask_MoveToRandomLoc : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToRandomLoc();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roam")
	float DestinationRadius = 1000.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FVector GetRandomLocation(APawn* ControlledPawn);
};
