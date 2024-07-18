// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "ATGameState.h"
#include "PlayerManager.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find Player Location";
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get the player manager to search for closest local or networked player
	if (APlayerManager* PlayerManager = GetWorld()->GetGameState<AATGameState>()->GetPlayerManager()) {
		if (AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner())) {
			if (APawn* const Pawn = Controller->GetPawn()) {
				FVector const Origin = Pawn->GetActorLocation();
				//get the closest player to this AI
				if (APlayerCharacter* player = PlayerManager->GetClosestPlayer(Origin)) {
					//get player location
					FVector const PlayerLocation = player->GetActorLocation();


					if (SearchRandom) {
						FNavLocation Loc;
						//get nav system and generate random location near player
						if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld())) {

							if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc)) {
								OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
								FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
								return EBTNodeResult::Succeeded;
							}
						}
					}
					else {
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
