// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class APOCTRAINNETWORKED_API UFlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFlashComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void GetMaterialsToFlash();

	TArray<UMaterialInstanceDynamic*> FlashMaterials;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor colorToFlash;

	UPROPERTY(EditDefaultsOnly)
	float FlashDuration;

	FTimerHandle flashTimerHandle;

	UFUNCTION()
	void ResetFlashColor();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Flash();
};
