// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToRandomLoc.h"
#include "NavigationSystem.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"


UBTTask_MoveToRandomLoc::UBTTask_MoveToRandomLoc()
{
	NodeName = "Move to Random Location";
}

EBTNodeResult::Type UBTTask_MoveToRandomLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;

	if (!EnemyPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector Destination = GetRandomLocation(EnemyPawn);
	if (Destination.IsZero())
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyPawn);
	EnemyCharacter->SetEnemyState(EEnemyState::chasing);
	EnemyController->MoveToLocation(Destination);

	return EBTNodeResult::Succeeded;
}

FVector UBTTask_MoveToRandomLoc::GetRandomLocation(APawn* EnemyPawn)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	
	if (!NavSys || !EnemyPawn)
	{
		return FVector::ZeroVector;
	}

	FVector EnemyOrigin = EnemyPawn->GetActorLocation();
	FNavLocation RandomLocation;

	bool bFoundLocation = NavSys->GetRandomReachablePointInRadius(EnemyOrigin, DestinationRadius, RandomLocation);
	if (bFoundLocation)
	{
		return RandomLocation.Location;
	}

	return FVector::ZeroVector;
}
