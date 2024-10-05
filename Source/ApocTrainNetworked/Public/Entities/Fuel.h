// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarryableActor.h"
#include "Fuel.generated.h"

/**
 * 
 */
UCLASS()
class APOCTRAINNETWORKED_API AFuel : public ACarryableActor
{
	GENERATED_BODY()
public:

	virtual void OnPhysicsComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override; 

	virtual void Server_DropObject_Implementation(FVector directionToLaunch, FVector DropperLocation) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFuelBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION(BlueprintCallable)
	void OnFuelDeposited();

	void CheckForDeposit();
};
