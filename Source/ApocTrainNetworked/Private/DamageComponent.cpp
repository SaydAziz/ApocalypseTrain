// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxHealth = 100.0f;
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}

void UDamageComponent::SetNegateDamageTrue()
{
	bNegateDamage = true;
}

void UDamageComponent::OnRep_Health()
{
	OnDamageTaken.Broadcast(previousDamageTaken);
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;
		OnDeath.Broadcast();
	}
}

float UDamageComponent::GetHealth()
{
	return CurrentHealth;
}

float UDamageComponent::GetMaxHealth()
{
	return MaxHealth;
}


void UDamageComponent::Damage(float damageToTake)
{
	if (bNegateDamage) {
		return;
	}
	if (GetOwner()->HasAuthority())
	{
		previousDamageTaken = damageToTake;
		CurrentHealth -= previousDamageTaken;
		OnRep_Health(); // Force update on clients
	}
	else
	{
		// If we're on a client, call the server function
		ServerDamage(damageToTake);
	}
}

void UDamageComponent::ServerDamage_Implementation(float damageToTake)
{
	previousDamageTaken = damageToTake;
	CurrentHealth -= previousDamageTaken;
	OnRep_Health();
}

void UDamageComponent::Reset()
{
	CurrentHealth = MaxHealth;
	bNegateDamage = false;
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDamageComponent, CurrentHealth)
	DOREPLIFETIME(UDamageComponent, previousDamageTaken)
}


