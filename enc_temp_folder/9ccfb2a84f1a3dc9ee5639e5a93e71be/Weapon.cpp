// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Root"));
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	PickUpTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUP Trigger"));

	RootComponent = WeaponRoot;
	WeaponMesh->SetupAttachment(RootComponent);
	PickUpTrigger->SetupAttachment(WeaponMesh);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::StartAttack()
{
}

void AWeapon::Attack()
{
}

void AWeapon::StopAttack()
{
}

void AWeapon::ResetAttack()
{
}

void AWeapon::Server_Attack_Implementation()
{

}

void AWeapon::Equip()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("EQUIPPED"));
	PickUpTrigger->SetActive(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetGenerateOverlapEvents(false);
	Highlight(false);
}

void AWeapon::Highlight(bool val)
{
	WeaponMesh->SetRenderCustomDepth(val);
}

