// Fill out your copyright notice in the Description page of Project Settings.


#include "FuelComponent.h"
#include "Components/BoxComponent.h"
#include "Fuel.h"
#include "PlayerManager.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UFuelComponent::UFuelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UFuelComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);

	for (TActorIterator<APlayerManager> IT(GetWorld()); IT; ++IT)
	{
		PlayerManager = *IT;
	}

	DepositTrigger = GetOwner()->FindComponentByTag<UBoxComponent>("FuelDeposit");
	MeshToHighlight = GetOwner()->FindComponentByTag<UStaticMeshComponent>("MeshToHighlight");

	if (DepositTrigger) {
		DepositTrigger->OnComponentBeginOverlap.AddDynamic(this, &UFuelComponent::OnDepositBeginOverlap);
	}
	
}

void UFuelComponent::OnDepositBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFuel* fuel = Cast<AFuel>(OtherActor)) {
		Server_AddFuel(fuel);
	}
}

// Called every frame
void UFuelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bBurning) {
		BurnFuel(DeltaTime);
	}
	if (PlayerManager && DepositTrigger) {
		if (PlayerManager->IsOverlappingPlayerWithFuel(DepositTrigger)) {
			Server_PlayersOverlapping(true);
		}
		else {
			Server_PlayersOverlapping(false);
		}
	}
}

bool UFuelComponent::IsInsideDeposit(FVector location)
{
	if (UKismetMathLibrary::IsPointInBox(location, DepositTrigger->GetComponentLocation(), DepositTrigger->GetScaledBoxExtent())) {
		return true;
	}
	return false;
}

bool UFuelComponent::IsFuelCritical()
{
	return CurrentFuel <= CriticalLevel && HasFuel();
}

bool UFuelComponent::HasFuel()
{
	return CurrentFuel > 0;
}

bool UFuelComponent::IsFull()
{
	return CurrentFuel >= MaxFuel;
}

void UFuelComponent::OnRep_PlayerWithFuelOverlappingUpdated()
{
	if (MeshToHighlight) {
		MeshToHighlight->SetRenderCustomDepth(bPlayerWithFuelOverlapping);
	}
}

void UFuelComponent::Server_PlayersOverlapping_Implementation(bool overlapping)
{
	Multi_PlayersOverlapping(overlapping);
}


void UFuelComponent::Multi_PlayersOverlapping_Implementation(bool overlapping)
{
	if (MeshToHighlight) {
		MeshToHighlight->SetRenderCustomDepth(overlapping);
	}
}

void UFuelComponent::Server_AddFuel_Implementation(AFuel* fuel)
{
	if (!IsFull() && !fuel->IsCarried()) {
		CurrentFuel += 1;
		fuel->Destroy();
	}
}

void UFuelComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFuelComponent, CurrentFuel)
}

void UFuelComponent::BurnFuel(float DeltaTime)
{
	CurrentFuel -= BurnRate * DeltaTime;
	if (CurrentFuel < 0) {
		CurrentFuel = 0;
	}
}

