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

	UPROPERTY(Replicated)
	TArray<UMaterialInstanceDynamic*> FlashMaterials;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ColorToFlash;

	UPROPERTY(ReplicatedUsing = OnRep_MaterialColor)
	FLinearColor MaterialColor;

	UFUNCTION()
	void OnRep_MaterialColor();

	UPROPERTY(EditDefaultsOnly)
	float FlashDuration;

	FTimerHandle flashTimerHandle;

	void ResetFlashColor();

	void SetMaterialParameter(FLinearColor NewParameter);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Flash();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
