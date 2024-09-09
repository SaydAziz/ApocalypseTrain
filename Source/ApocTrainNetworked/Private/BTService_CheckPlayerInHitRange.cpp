// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPlayerInHitRange.h"
#include "EnemyAIController.h"
#include "PlayerManager.h"
#include "PlayerCharacter.h"
#include "ATGameState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckPlayerInHitRange::UBTService_CheckPlayerInHitRange()
{
	NodeName = "Check Player In Hit Range";
}

void UBTService_CheckPlayerInHitRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
				BlackboardComp->SetValueAsBool(PlayerInHitRangeKey, bIsPlayerInRange);
			}

		}
	}
}
