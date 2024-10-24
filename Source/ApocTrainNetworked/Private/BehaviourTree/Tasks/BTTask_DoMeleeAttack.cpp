// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTree/Tasks/BTTask_DoMeleeAttack.h"
#include "Enemies/EnemyAIController.h"
#include "Enemies/EnemyCharacter.h"

UBTTask_DoMeleeAttack::UBTTask_DoMeleeAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Execute Melee Attack";
}

EBTNodeResult::Type UBTTask_DoMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AEnemyAIController* controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(controller->GetPawn()))
	{
		if (enemy->CanAttack()) 
		{
			enemy->ExecuteMeleeAttack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
