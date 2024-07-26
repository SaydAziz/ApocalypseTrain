// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Poolable.h"
#include "ObjectPooler.generated.h"

UCLASS()
class APOCTRAINNETWORKED_API AObjectPooler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectPooler();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
	int TotalObjects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning)
	FVector InitialPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spawning)
	TSubclassOf<AActor> ObjectType;

	TQueue<IPoolable*> Pool;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateObjects();

	FVector GetSpawnLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnObject(FVector SpawnLocation);

	virtual void InitializePoolerFromSpawn(TSubclassOf<AActor> Type, int Total);

};
