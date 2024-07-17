// Fill out your copyright notice in the Description page of Project Settings.


#include "PulseComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values for this component's properties
UPulseComponent::UPulseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PulseTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PulseTimeline"));
	// ...
}


// Called when the game starts
void UPulseComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	GetMeshesToPulse();
	// ...
	if (FloatCurve)
	{
		FOnTimelineFloat TimelineProgress;
		PulseTimeline->SetIsReplicated(true);
		TimelineProgress.BindUFunction(this, FName("HandleProgress"));
		PulseTimeline->AddInterpFloat(FloatCurve, TimelineProgress);
		PulseTimeline->SetLooping(false);
		PulseTimeline->SetIgnoreTimeDilation(true);
	}
}

void UPulseComponent::GetMeshesToPulse()
{
	TSet<UActorComponent*> components = GetOwner()->GetComponents();
	for (UActorComponent* component : components) {
		if (UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(component)) {
			if (mesh->ComponentHasTag("Pulse")) {
				mesh->SetIsReplicated(true);
				pulseMeshes.Add(mesh);
			}
		}
	}
	for (UStaticMeshComponent* mesh : pulseMeshes) {
		startingScales.Add(mesh->GetComponentScale());
	}
}


void UPulseComponent::HandleProgress(float Value)
{
	for (int i = 0; i < pulseMeshes.Num(); i++) {
		pulseMeshes[i]->SetRelativeScale3D(startingScales[i] * Value);
	}
}

// Called every frame
void UPulseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPulseComponent::Pulse()
{
	PulseTimeline->PlayFromStart();
}

