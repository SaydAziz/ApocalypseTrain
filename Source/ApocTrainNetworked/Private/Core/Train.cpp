// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Train.h"
#include "Net/UnrealNetwork.h"
#include "ATComponents/FuelComponent.h"
#include "Components/BoxComponent.h"
#include "ATComponents/PulseComponent.h"
#include "ATComponents/FlashComponent.h"
#include "Core/ATGameState.h"

// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	fuelComponent = CreateDefaultSubobject<UFuelComponent>(TEXT("FuelComponent"));
	pulseComponent = CreateDefaultSubobject<UPulseComponent>(TEXT("PulseComponent"));
	flashComponent = CreateDefaultSubobject<UFlashComponent>(TEXT("FlashComponent"));
}


// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();
	gameState = GetWorld()->GetGameState<AATGameState>();
	startingFuel = fuelComponent->CurrentFuel;
}


// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanMove) {
		UpdateSpeed(DeltaTime);
		UpdateLocation(DeltaTime);
	}
	UpdateFuelComponent(DeltaTime);
	CheckGameState();
}

void ATrain::CheckGameState()
{
	switch (gameState->GetGameState()) {
		case EGameState::lobby:
			if (fuelComponent->CurrentFuel > startingFuel) {
				gameState->SetGameState(EGameState::traveling);
			}
			break;
		case EGameState::traveling:
			if (IsTrainStopped() || IsStopping()) {
				StartTrain();
			}
			break;
		case EGameState::Encounter:
			if (!IsStopping()) {
				StopTrain();
			}
			break;

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
}


void ATrain::UpdateLocation(float DeltaTime)
{
	currentLocation = GetActorLocation();
	currentLocation += FVector(0, 1, 0) * currentTrainSpeed * DeltaTime;
	SetActorLocation(currentLocation, true);
}

void ATrain::UpdateFuelComponent(float DeltaTime)
{
	if (currentTrainState != ETrainState::stopped && currentTrainState != ETrainState::decelerating) {
		fuelComponent->bBurning = false;
	}
	else {
		fuelComponent->bBurning = true;
	}
	if (!fuelComponent->HasFuel()) {
		SetTrainState(ETrainState::decelerating);
	}
}


bool ATrain::IsStopping()
{
	return currentTrainState == ETrainState::decelerating;
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

void ATrain::StartTrain()
{
	SetTrainState(ETrainState::accelerating);
}

void ATrain::StopTrain()
{
	SetTrainState(ETrainState::decelerating);
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
	
}

float ATrain::GetCurrentFuel()
{
	return fuelComponent->CurrentFuel;
}

float ATrain::GetMaxFuel()
{
	return fuelComponent->MaxFuel;
}

bool ATrain::IsGameOverCountingDown()
{
	return false;
}

FVector ATrain::GetTrainLocation()
{
	return currentLocation;
}
