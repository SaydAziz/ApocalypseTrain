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

void AProjectileWeapon::Attack()
{
	FHitResult* HitResult = new FHitResult();

	FVector StartTrace = GetParentActor()->GetActorLocation();
	FVector ForwardVector = GetParentActor()->GetActorForwardVector();
	FVector EndTrace = ((ForwardVector * 2000.0f) + StartTrace);

	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	TraceParams->AddIgnoredActor(GetParentActor());

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Pawn, *TraceParams))
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.0f);


	}


}

void AProjectileWeapon::Reload()
{
}

void AProjectileWeapon::Equip()
{
}


