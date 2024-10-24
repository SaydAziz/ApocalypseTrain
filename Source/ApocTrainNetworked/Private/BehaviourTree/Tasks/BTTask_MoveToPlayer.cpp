// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTree/Tasks/BTTask_MoveToPlayer.h"
#include "Enemies/EnemyAIController.h"
#include "Core/ATGameState.h"
#include "Core/PlayerManager.h"
#include "Core/PlayerCharacter.h"
#include "Enemies/EnemyCharacter.h"
#include "NavigationSystem.h"

UBTTask_MoveToPlayer::UBTTask_MoveToPlayer()
{
	NodeName = "Move To Player";
}

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	APawn* EnemyPawn = EnemyController ? EnemyController->GetPawn() : nullptr;
	APlayerManager* PlayerManager = GetWorld()->GetGameState<AATGameState>()->GetPlayerManager();

	if (!EnemyPawn || !PlayerManager)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyPawn);

	FVector EnemyOrigin = EnemyPawn->GetActorLocation();

	if (APlayerCharacter* TargetPlayer = PlayerManager->GetClosestPlayer(EnemyOrigin))
	{
		EnemyCharacter->SetEnemyState(EEnemyState::chasing);
		APawn* TargetPawn = TargetPlayer->GetController()->GetPawn();
		MoveToPlayer(EnemyPawn, TargetPawn);
			
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_MoveToPlayer::MoveToPlayer(APawn* EnemyPawn, APawn* PlayerPawn)
{
	AEnemyAIController* EnemyController = Cast<AEnemyAIController>(EnemyPawn->GetController());
	if (EnemyController)
	{
		EnemyController->MoveToActor(PlayerPawn, 3.0f);
	}
}
