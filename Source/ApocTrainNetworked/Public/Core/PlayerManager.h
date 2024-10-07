// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerManager.generated.h"

class APlayerCharacter;

UCLASS()
class APOCTRAINNETWORKED_API APlayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerManager();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ATrain* train;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Spawning)
	FVector playerDeathPosition;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetPlayerDeathPos();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<APlayerCharacter*> ActivePlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FColor> PlayerColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Spawning")
	FVector SpawnOffset;

	UFUNCTION(BlueprintCallable)
	int RegisterPlayerWithManager(class APlayerCharacter* joinedPlayer);

	bool IsOverlappingPlayer(class UBoxComponent* box);

	bool IsOverlappingPlayerWithFuel(class UBoxComponent* box);

	APlayerCharacter* GetClosestPlayer(FVector location);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetPlayerSpawnPoint(int PlayerIndex);

	bool IsOutOfBounds(FVector location);

};
