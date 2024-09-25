// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class APOCTRAINNETWORKED_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly)
	class UPulseComponent* pulseComponent;

	UPROPERTY(EditDefaultsOnly)
	class UFlashComponent* flashComponent;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* DestroyedDust;

	UPROPERTY(EditDefaultsOnly)
	class UDamageComponent* DamageComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TakeDamage(float damageToTake);
	UFUNCTION()
	virtual void OnObjectDeath();


};
