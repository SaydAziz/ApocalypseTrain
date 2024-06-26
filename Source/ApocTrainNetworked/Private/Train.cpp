// Fill out your copyright notice in the Description page of Project Settings.


#include "Train.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}

void ATrain::StartTrain()
{
	SetTrainState(ETrainState::accelerating);
}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove) {
		UpdateSpeed(DeltaTime);
		UpdateLocation(DeltaTime);
	}
}


void ATrain::UpdateSpeed(float deltaTime)
{
	switch (currentTrainState) {
	case ETrainState::stopped:
		if (!IsGameOverCountingDown()) {
			SetTrainState(ETrainState::accelerating);
		}
		break;
	case ETrainState::accelerating:
		if (currentTrainSpeed < MaxTrainSpeed) {
			currentTrainSpeed += AccelerationRate * deltaTime;
		}
		break;
	case ETrainState::decelerating:
		if (isReversing) {
			if (currentTrainSpeed < 0) {
				currentTrainSpeed += DecelerationRate * deltaTime;
			}
			else {
				isReversing = false;
			}
		}
		else {
			if (currentTrainSpeed > 0) {
				currentTrainSpeed -= DecelerationRate * deltaTime;
			}
			else {
				SetTrainState(ETrainState::stopped);
				currentTrainSpeed = 0;
			}
		}
		break;
	default:
		break;
	}
	if (currentTrainState != ETrainState::stopped && currentTrainState != ETrainState::decelerating) {
		//BurnFuel();
	}
}


void ATrain::UpdateLocation(float DeltaTime)
{
	currentLocation = GetActorLocation();
	currentLocation += FVector(0, 1, 0) * currentTrainSpeed * DeltaTime;
	SetActorLocation(currentLocation, true);
}


void ATrain::SetTrainState(ETrainState stateToSet)
{
	switch (stateToSet) {
	case ETrainState::stopped:
		NotifyTrainStop();
		break;
	case ETrainState::accelerating:
		NotifyTrainStart();
		break;
	case ETrainState::decelerating:
		break;
	}
	currentTrainState = stateToSet;
}

bool ATrain::IsTrainStopped()
{
	if (currentTrainState == ETrainState::stopped) {
		return true;
	}
	return false;
}

void ATrain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ATrain, currentLocation)
}

bool ATrain::IsGameOverCountingDown()
{
	return false;
}

FVector ATrain::GetTrainLocation()
{
	return currentLocation;
}
