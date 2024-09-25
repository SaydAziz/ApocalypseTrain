// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Obstacle.h"
#include "ATComponents/PulseComponent.h"
#include "ATComponents/FlashComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ATComponents/DamageComponent.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pulseComponent = CreateDefaultSubobject<UPulseComponent>(TEXT("PulseComponent"));
	flashComponent = CreateDefaultSubobject<UFlashComponent>(TEXT("FlashComponent"));

	DamageComponent = CreateDefaultSubobject<UDamageComponent>("Damage Component");
	DamageComponent->SetIsReplicated(true);
	DamageComponent->OnDamageTaken.AddDynamic(this, &AObstacle::TakeDamage);
	DamageComponent->OnDeath.AddDynamic(this, &AObstacle::OnObjectDeath);

}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	bAlwaysRelevant = true;
	DamageComponent->MaxHealth = MaxHealth;
	DamageComponent->Reset();
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

void AObstacle::TakeDamage(float damageToTake)
{
	if (pulseComponent) {
		pulseComponent->Pulse();
	}
	if (flashComponent) {
		flashComponent->Flash();
	}
}

void AObstacle::OnObjectDeath()
{
	Destroy();
}


