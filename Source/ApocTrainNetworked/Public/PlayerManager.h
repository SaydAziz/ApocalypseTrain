// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerManager.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class APlayerCharacter*> ActivePlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FColor> PlayerColors;

	UFUNCTION(BlueprintCallable)

	int RegisterPlayerWithManager(class APlayerCharacter* joinedPlayer);

	bool IsOverlappingPlayer(class UBoxComponent* box);

	bool IsOverlappingPlayerWithFuel(class UBoxComponent* box);

};
