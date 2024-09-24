// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken,float, damageToTake);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class APOCTRAINNETWORKED_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(ReplicatedUsing= OnRep_Health)
	float CurrentHealth;

	UPROPERTY(Replicated)
	float previousDamageTaken;
	// Function that will be called on clients when Health is updated
	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditDefaultsOnly);
	float MaxHealth;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth();
	
	UFUNCTION(BlueprintCallable)
	void Damage(float damageToTake);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDamage(float damageToTake);
	void ServerDamage_Implementation(float damageToTake);


	void Reset();

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FOnDamageTaken OnDamageTaken;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FOnDeath OnDeath;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
