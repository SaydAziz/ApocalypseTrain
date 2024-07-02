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



	virtual void Server_DropObject_Implementation(FVector directionToLaunch, FVector DropperLocation) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFuelBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY()
	class UFuelComponent* LastOverlappedDeposit;

	UFUNCTION(BlueprintCallable)
	void OnFuelDeposited();
};
