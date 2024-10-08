// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "ATComponents/DamageComponent.h"

AProjectileWeapon::AProjectileWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsHolding = false;
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("STARTED ATTACKING")));
	bIsHolding = true;
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
	bIsHolding = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("STOPPED ATTACKING")));
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
		StartTrace += GetAttachParentActor()->GetActorRightVector() * 20.0f;
		FVector ForwardVector = GetAttachParentActor()->GetActorForwardVector();
		//FVector StartTrace = GetActorLocation();
		//FVector ForwardVector = GetActorForwardVector();
		FVector EndTrace = ((ForwardVector * 6000.0f) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		TraceParams->AddIgnoredActor(GetAttachParentActor());
		TraceParams->AddIgnoredActor(this);

		FCollisionObjectQueryParams objectParams;
		objectParams.AddObjectTypesToQuery(ECC_Pawn);
		objectParams.AddObjectTypesToQuery(ECC_Destructible);
		TArray<FHitResult> hits;
		if (GetWorld()->LineTraceMultiByObjectType(hits, StartTrace, EndTrace, objectParams, *TraceParams)) {
			for (int i = 0; i < hits.Num(); i++) {
				if (i >= Data->Penetration) {
					break;
				}
				if (hits[i].GetActor()) {
					if (UDamageComponent* dmgComp = hits[i].GetActor()->GetComponentByClass<UDamageComponent>()) {
						dmgComp->Damage(Data->Damage);
					}
				}
			}
		}
		OnAttack.Broadcast();
		Multicast_AttackEffects();
	}
}

void AProjectileWeapon::Multicast_AttackEffects_Implementation() 
{
	if (Data->BulletTracer)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Data->BulletTracer, GetAttachParentActor()->GetActorLocation() + GetAttachParentActor()->GetActorRightVector() * 20.0f, GetAttachParentActor()->GetActorForwardVector().Rotation());
	}
}

void AProjectileWeapon::ResetAttack()
{
	SetWeaponState(EProjectileWeaponState::idle);
	if (bIsHolding == true)
	{
		StartAttack();
	}
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


