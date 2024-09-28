// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ATGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnEnemies, float, ypos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIncreaseDifficulty);


UENUM(BlueprintType)
enum class EGameState : uint8
{
	lobby UMETA(DisplayName = "Lobby"), traveling UMETA(DisplayName = "Traveling"), Encounter UMETA(DisplayName = "Encounter")
};
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
	FIncreaseDifficulty OnDifficultyIncrease;

	UFUNCTION()
	void OnChunkSpawned(struct FSpawnedChunkInfo info);

	UFUNCTION(BlueprintCallable)
	void SetGameState(EGameState stateToSet);

	UFUNCTION(BlueprintCallable)
	EGameState GetGameState();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentGameState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerManager* playerManager;

};


//gamestate subscribe to chunkspawner, gamestate 
//new chunk spawned event, increment counter, call it on everything on something else

//spawn unique level chunks