// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "EnemyCharacter.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AEnemyCharacter* const enemy = Cast<AEnemyCharacter>(InPawn)) {
		if (UBehaviorTree* const tree = enemy->GetBehaviorTree()) {
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}
