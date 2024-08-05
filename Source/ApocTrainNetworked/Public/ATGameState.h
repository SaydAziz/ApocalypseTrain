// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ATGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnEnemies, float, ypos);

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AATGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerManager* GetPlayerManager();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FSpawnEnemies OnSpawnEnemies;

	UFUNCTION()
	void OnChunkSpawned(struct FSpawnedChunkInfo info);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerManager* playerManager;

};


//gamestate subscribe to chunkspawner, gamestate 
//new chunk spawned event, increment counter, call it on everything on something else

//spawn unique level chunks