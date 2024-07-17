// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "PulseComponent.h"
#include "FlashComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pulseComponent = CreateDefaultSubobject<UPulseComponent>(TEXT("PulseComponent"));
	flashComponent = CreateDefaultSubobject<UFlashComponent>(TEXT("FlashComponent"));

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	bAlwaysRelevant = true;
	currentHealth = MaxHealth;
}

void AObstacle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (EndPlayReason)
	{
		case EEndPlayReason::Destroyed:
			if (DestroyedDust) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyedDust, GetActorLocation(), GetActorRotation())->SetIsReplicated(true);
			}
			break;
	}
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::Damage(float damageToTake)
{
	currentHealth -= damageToTake;
	if (pulseComponent) {
		pulseComponent->Pulse();
	}
	if (flashComponent) {
		flashComponent->Flash();
	}
	if (currentHealth <= 0) {
		Destroy();
	}
}

float AObstacle::GetHealth()
{
	return currentHealth;
}

