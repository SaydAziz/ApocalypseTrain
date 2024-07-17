// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "PulseComponent.h"
#include "FlashComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	pulseComponent = CreateDefaultSubobject<UPulseComponent>(TEXT("PulseComponent"));
	flashComponent = CreateDefaultSubobject<UFlashComponent>(TEXT("FlashComponent"));

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = MaxHealth;
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
		if (DestroyedDust) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyedDust, GetActorLocation(), GetActorRotation());
		}
		Destroy(0);
	}
}

float AObstacle::GetHealth()
{
	return currentHealth;
}

