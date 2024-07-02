// Fill out your copyright notice in the Description page of Project Settings.


#include "Fuel.h"
#include "FuelComponent.h"
#include "Components/SphereComponent.h"
#include <Net/UnrealNetwork.h>


//on overlap deposit calls method deposit fuel 

//player calls on dropped method on fuel, which checks if it is overlapping a component for deposit fuel

//player store reference to the overlapped trigger



void AFuel::Server_DropObject_Implementation(FVector directionToLaunch, FVector DropperLocation)
{
	Super::Server_DropObject_Implementation(directionToLaunch, DropperLocation);
	if (LastOverlappedDeposit) {
		if (LastOverlappedDeposit->IsInsideDeposit(DropperLocation)) {
			LastOverlappedDeposit->Server_AddFuel(this);
		}
	}
	
}

void AFuel::BeginPlay()
{
	Super::BeginPlay();
	if (trigger) {
		trigger->OnComponentBeginOverlap.AddDynamic(this, &AFuel::OnFuelBeginOverlap);
	}
}

//do this stuff in the player, also make sure you can set render depth buffer on the deposit mesh

void AFuel::OnFuelBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0, FColor::Purple, OtherComp->GetName());
		if (UFuelComponent* fuelComponent = OtherActor->FindComponentByClass<UFuelComponent>()) {
			LastOverlappedDeposit = fuelComponent;
		}
	}
}

void AFuel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AFuel, LastOverlappedDeposit)
}

void AFuel::OnFuelDeposited()
{

}
