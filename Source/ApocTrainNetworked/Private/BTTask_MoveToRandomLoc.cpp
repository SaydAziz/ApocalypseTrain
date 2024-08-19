// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToRandomLoc.h"
#include "NavigationSystem.h"
#include "EnemyAIController.h"


UBTTask_MoveToRandomLoc::UBTTask_MoveToRandomLoc()
{
	NodeName = "Move to Random Location";
}

EBTNodeResult::Type UBTTask_MoveToRandomLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* ControlledPawn = EnemyController ? EnemyController->GetPawn() : nullptr;

	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Destination = GetRandomLocation(ControlledPawn);
	if (Destination.IsZero())
	{
		return EBTNodeResult::Failed;
	}

	EnemyController->MoveToLocation(Destination);

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_MoveToRandomLoc::GetRandomLocation(APawn* ControlledPawn)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSys || !ControlledPawn)
	{
		return FVector::ZeroVector;
	}

	FVector EnemyOrigin = ControlledPawn->GetActorLocation();
	FNavLocation RandomLocation;

	bool bFoundLocation = NavSys->GetRandomReachablePointInRadius(EnemyOrigin, DestinationRadius, RandomLocation);
	if (bFoundLocation)
	{
		return RandomLocation.Location;
	}

	return FVector::ZeroVector;
}
