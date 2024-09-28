// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Fuel.h"
#include "ATComponents/FuelComponent.h"
#include "Components/SphereComponent.h"
#include <Net/UnrealNetwork.h>


//on overlap deposit calls method deposit fuel 

//player calls on dropped method on fuel, which checks if it is overlapping a component for deposit fuel

//player store reference to the overlapped trigger



void AFuel::Server_DropObject_Implementation(FVector directionToLaunch, FVector DropperLocation)
{
	//this is bad - if player has not entered fuel box with fuel yet this will not work bc last overlap deposit is null need to find a new method
	Super::Server_DropObject_Implementation(directionToLaunch, DropperLocation);
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor) {
			if (UFuelComponent* fuelComp = Actor->FindComponentByClass<UFuelComponent>())
			{
				fuelComp->Server_AddFuel(this);
			}
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

}

void AFuel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AFuel::OnFuelDeposited()
{

}
