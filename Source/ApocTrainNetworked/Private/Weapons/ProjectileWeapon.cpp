// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "ATComponents/DamageComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

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
		for (int i = 0; i < Data->ShotsPerAttack; i++) {
			ShootProjectile();
		}
	}
}

void AProjectileWeapon::Multicast_AttackEffects_Implementation() 
{
	if (Data->BulletTracer)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Data->BulletTracer, GetAttachParentActor()->GetActorLocation() + GetAttachParentActor()->GetActorRightVector() * 20.0f, GetAttachParentActor()->GetActorForwardVector().Rotation());
	}
	if (Data->MuzzleFlash) 
	{
		FVector Location = WeaponMesh->GetSocketLocation("FlashSocket");
		FRotator Rotation = GetAttachParentActor()->GetActorForwardVector().Rotation();
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),Data->MuzzleFlash,Location,Rotation);
		// Optionally, you can set custom parameters (like direction) on the spawned Niagara system
		if (NiagaraComp)
		{
			NiagaraComp->SetVectorParameter(FName("CustomDirection"), GetAttachParentActor()->GetActorForwardVector());
		}
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

void AProjectileWeapon::ShootProjectile()
{
	FHitResult HitResult = FHitResult();

	FVector StartTrace = GetAttachParentActor()->GetActorLocation();
	StartTrace += GetAttachParentActor()->GetActorRightVector() * 20.0f;
	FVector ForwardVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(GetAttachParentActor()->GetActorForwardVector(), Data->RandomShotConeRadius);
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


