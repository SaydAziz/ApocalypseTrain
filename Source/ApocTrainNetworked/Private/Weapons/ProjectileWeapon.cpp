// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "ATComponents/DamageComponent.h"

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
	if (!HasAuthority())
	{
		GetWorldTimerManager().SetTimer(CanAttackTimerHandle, this, &AProjectileWeapon::ResetAttack, Data->AttackRate, false);
		Server_Attack();
	}
	else
	{
		GetWorldTimerManager().SetTimer(CanAttackTimerHandle, this, &AProjectileWeapon::ResetAttack, Data->AttackRate, false);
		FHitResult HitResult = FHitResult();

		FVector StartTrace = GetAttachParentActor()->GetActorLocation();
		FVector ForwardVector = GetAttachParentActor()->GetActorForwardVector();
		FVector EndTrace = ((ForwardVector * 6000.0f) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		TraceParams->AddIgnoredActor(GetAttachParentActor());
		TraceParams->AddIgnoredActor(this);

		//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.0f);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Pawn, *TraceParams))
		{
			if (HitResult.GetActor()) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
				if (UDamageComponent* dmgComp = HitResult.GetActor()->GetComponentByClass<UDamageComponent>()) {
					dmgComp->Damage(Data->Damage);
				}
				/*if (IDamagable* damagableActor = Cast<IDamagable>(HitResult.GetActor())) {
					damagableActor->Damage(Data->Damage);
				}*/
			}
		}

		Multicast_AttackEffects();
	}
}

void AProjectileWeapon::Multicast_AttackEffects_Implementation() 
{
	if (Data->BulletTracer)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Data->BulletTracer, WeaponMesh->GetSocketLocation("MuzzleSocket"), GetAttachParentActor()->GetActorForwardVector().Rotation());
	}
}

void AProjectileWeapon::ResetAttack()
{
	SetWeaponState(EProjectileWeaponState::idle);
}

void AProjectileWeapon::Reload()
{
}

void AProjectileWeapon::Server_Attack_Implementation()
{
	Attack();
}


void AProjectileWeapon::Equip()
{
	Super::Equip();
}

void AProjectileWeapon::SetWeaponState(EProjectileWeaponState NewWeaponState)
{
	if (CurrentWeaponState != NewWeaponState)
	{
		CurrentWeaponState = NewWeaponState;
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Switched to Weapon State: %d"), CurrentWeaponState));

		//Add transition logic here
	}
}


