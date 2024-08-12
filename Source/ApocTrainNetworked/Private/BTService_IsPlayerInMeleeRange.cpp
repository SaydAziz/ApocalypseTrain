// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <PlayerManager.h>
#include "ATGameState.h"
#include "PlayerCharacter.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	const AEnemyAIController* controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemyCharacter* enemy = Cast<AEnemyCharacter>(controller->GetPawn());
	if (APlayerManager* PlayerManager = GetWorld()->GetGameState<AATGameState>()->GetPlayerManager()) {
		FVector const Origin = enemy->GetActorLocation();
		//get the closest player to this AI
		if (APlayerCharacter* player = PlayerManager->GetClosestPlayer(Origin)) {
			//get player location
			FVector const PlayerLocation = player->GetActorLocation();
			float const attackRadius = enemy->GetAttackRadius();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->GetDistanceTo(player) <= attackRadius);
		}
	}
}
