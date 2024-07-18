// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get AI controller and its enemy
	if (AEnemyAIController* const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
		if (APawn* const pawn = cont->GetPawn()) {
			FVector const Origin = pawn->GetActorLocation();
			if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())) {
				FNavLocation loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, loc)) {
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), loc.Location);

				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
