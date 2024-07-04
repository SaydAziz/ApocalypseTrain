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
	Attack();
	GetWorldTimerManager().SetTimer(AttackRateTimerHandle, this, &AProjectileWeapon::Attack, AttackRate, true);
}

void AProjectileWeapon::StopAttack()
{
	GetWorldTimerManager().ClearTimer(AttackRateTimerHandle);
}

void AProjectileWeapon::Attack()
{
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

void AProjectileWeapon::Reload()
{
}

void AProjectileWeapon::Equip()
{
}


