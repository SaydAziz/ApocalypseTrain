// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UFlashComponent::UFlashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UFlashComponent::BeginPlay()
{
	Super::BeginPlay();
	GetMaterialsToFlash();
}

void UFlashComponent::GetMaterialsToFlash()
{
	TSet<UActorComponent*> components = GetOwner()->GetComponents();
	for (UActorComponent* component : components) {
		if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(component)) {
			if (mesh->ComponentHasTag("Flash")) {
				mesh->SetIsReplicated(true);
				UMaterialInterface* baseMat = mesh->GetMaterial(0);
				UMaterialInstanceDynamic* DynamicMaterialInstance = mesh->CreateDynamicMaterialInstance(0, baseMat);
				mesh->SetMaterial(0, DynamicMaterialInstance);
				FlashMaterials.Add(DynamicMaterialInstance);
			}
		}
	}
}


void UFlashComponent::ResetFlashColor()
{
	for (UMaterialInstanceDynamic* mat : FlashMaterials) {
		mat->SetVectorParameterValue(FName("FlashVector"), FVector(0,0,0));
	}
}

// Called every frame
void UFlashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFlashComponent::Flash()
{
	for (UMaterialInstanceDynamic* mat : FlashMaterials) {
		mat->SetVectorParameterValue(FName("FlashVector"), colorToFlash);
	}
	GetWorld()->GetTimerManager().SetTimer(flashTimerHandle, this, &UFlashComponent::ResetFlashColor, FlashDuration, false);
}

