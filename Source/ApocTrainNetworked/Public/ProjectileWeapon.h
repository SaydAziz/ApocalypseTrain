// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

UENUM()
enum class EProjectileWeaponState: uint8
{
	idle UMETA(DisplayName = "Idle"), shooting UMETA(DisplayName = "Shooting"), reloading UMETA(DisplayName = "Reloading")
};

UCLASS()
class APOCTRAINNETWORKED_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Equip() override;

	EProjectileWeaponState currentWeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	float ClipAmmoAmount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartAttack() override;
	virtual void StopAttack() override;
	virtual void Attack() override;
	
	virtual void Reload();

};
