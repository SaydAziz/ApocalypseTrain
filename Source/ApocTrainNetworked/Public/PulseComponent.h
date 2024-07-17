// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PulseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class APOCTRAINNETWORKED_API UPulseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPulseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void GetMeshesToPulse();
	TArray<UStaticMeshComponent*> pulseMeshes;
	TArray<FVector> startingScales;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
	class UTimelineComponent* PulseTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timeline")
	class UCurveFloat* FloatCurve;

	UFUNCTION()
	void HandleProgress(float Value);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Pulse();
};
