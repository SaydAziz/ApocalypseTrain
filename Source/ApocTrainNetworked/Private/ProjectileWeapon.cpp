// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

AProjectileWeapon::AProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileWeapon::StartAttack()
{
	if (CurrentWeaponState != EProjectileWeaponState::shooting)
	{
		SetWeaponState(EProjectileWeaponState::shooting);
		Attack();
		GetWorldTimerManager().SetTimer(AttackRateTimerHandle, this, &AProjectileWeapon::Attack, Data->AttackRate, true);
	}
}

void AProjectileWeapon::StopAttack()
{
	GetWorldTimerManager().ClearTimer(AttackRateTimerHandle);
}

void AProjectileWeapon::Attack()
{
	GetWorldTimerManager().SetTimer(CanAttackTimerHandle, this, &AProjectileWeapon::ResetAttack, Data->AttackRate, false);
	FHitResult* HitResult = new FHitResult();

	FVector StartTrace = GetAttachParentActor()->GetActorLocation();
	FVector ForwardVector = GetAttachParentActor()->GetActorForwardVector();
	FVector EndTrace = ((ForwardVector * 6000.0f) + StartTrace);

	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	TraceParams->AddIgnoredActor(GetAttachParentActor());
	TraceParams->AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.0f);

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Pawn, *TraceParams))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *HitResult->GetActor()->GetName()));
	}


}

void AProjectileWeapon::ResetAttack()
{
	SetWeaponState(EProjectileWeaponState::idle);
}

void AProjectileWeapon::Reload()
{
}

void AProjectileWeapon::Equip()
{
}

void AProjectileWeapon::SetWeaponState(EProjectileWeaponState NewWeaponState)
{
	if (CurrentWeaponState != NewWeaponState)
	{
		CurrentWeaponState = NewWeaponState;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Switched to Weapon State: %d"), CurrentWeaponState));

		//Add transition logic here
	}
}


