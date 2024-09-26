// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"


UENUM()
enum class ETrainState : uint8 {
	stopped UMETA(DIsplayName = "Stopped"), accelerating UMETA(DIsplayName = "Accelerating"), decelerating UMETA(DIsplayName = "Decelerating")
};


UCLASS()
class APOCTRAINNETWORKED_API ATrain : public AActor
{
	GENERATED_BODY()
	
public:	

	ATrain();
	FVector GetTrainLocation();
	bool IsTrainStopped();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentFuel();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxFuel();

protected:

	class AATGameState* gameState;

	bool isReversing;
	UPROPERTY()
	FVector currentLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETrainState currentTrainState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxTrainSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AccelerationRate;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DecelerationRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool CanMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float currentTrainSpeed;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CheckGameState();

	bool IsGameOverCountingDown();
	void UpdateSpeed(float deltaTime);
	void UpdateLocation(float DeltaTime);
	void UpdateFuelComponent(float DeltaTime);

	bool IsStopping();
	UFUNCTION()
	void SetTrainState(ETrainState stateToSet);
	UFUNCTION()
	void StopTrain();
	UFUNCTION(BlueprintCallable)
	void StartTrain();
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTrainStart();
	UFUNCTION(BlueprintImplementableEvent)
	void NotifyTrainStop();

	UPROPERTY(EditDefaultsOnly)
	class UFuelComponent* fuelComponent;

	UPROPERTY(EditDefaultsOnly)
	class UPulseComponent* pulseComponent;

	UPROPERTY(EditDefaultsOnly)
	class UFlashComponent* flashComponent;

};
