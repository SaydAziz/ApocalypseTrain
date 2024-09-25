// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTree/Services/BTService_CheckPlayerInRange.h"
#include "Enemies/EnemyAIController.h"
#include "Core/PlayerManager.h"
#include "Core/PlayerCharacter.h"
#include "Core/ATGameState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckPlayerInRange::UBTService_CheckPlayerInRange()
{
	NodeName = "Check Player In Range";
}

void UBTService_CheckPlayerInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;

	if (!EnemyPawn)
	{
		return;
	}

	FVector EnemyOrigin = EnemyPawn->GetActorLocation();

	if (APlayerManager* PlayerManager = GetWorld()->GetGameState<AATGameState>()->GetPlayerManager())
	{
		if (APlayerCharacter* TargetPlayer = PlayerManager->GetClosestPlayer(EnemyOrigin))
		{
			APawn* TargetPawn = TargetPlayer->GetController()->GetPawn();
			float DistanceToTarget = FVector::Dist(TargetPawn->GetActorLocation(), EnemyPawn->GetActorLocation());

			UBlackboardComponent* BlackboardComp = EnemyController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				bool bIsPlayerInRange = DistanceToTarget <= Range;
				BlackboardComp->SetValueAsBool(PlayerInRangeKey, bIsPlayerInRange);
			}

		}
	}
}
