// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WeaponData.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);

UCLASS(Abstract)
class APOCTRAINNETWORKED_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* WeaponRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* PickUpTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UWeaponData* Data;

	FTimerHandle AttackRateTimerHandle;
	FTimerHandle CanAttackTimerHandle;

	

public:	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAttack OnAttack;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Equip();
	virtual void StartAttack();
	virtual void Attack();
	virtual void StopAttack();
	virtual void ResetAttack();
	void Highlight(bool val);

	UFUNCTION(Server, Reliable)
	virtual void Server_Attack();
};
