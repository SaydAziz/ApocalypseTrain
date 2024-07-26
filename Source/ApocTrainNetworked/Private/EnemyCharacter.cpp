// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "FlashComponent.h"
#include "EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FlashComponent = CreateDefaultSubobject<UFlashComponent>("Flash Component");

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	InitializeEnemy();
}

void AEnemyCharacter::InitializeEnemy()
{
	if (GetController<AEnemyAIController>()) {
		AIController = GetController<AEnemyAIController>();
	}
	else {
		AIController = GetWorld()->SpawnActor<AEnemyAIController>();
		AIController->Possess(this);
	}
	AIController->SetSightSenseValues(EnemyData->SightRadius);
	GetCharacterMovement()->MaxWalkSpeed = EnemyData->Speed;
	currentHealth = EnemyData->Health;
	bIsDead = true;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AEnemyCharacter::GetBehaviorTree() const
{
	return Tree;
}

void AEnemyCharacter::Damage(float damageToTake)
{
	if (FlashComponent) {
		FlashComponent->Flash();
	}
	currentHealth -= damageToTake;
	if (currentHealth <= 0) {
		currentHealth = 0;
		OnDespawn();
	}
}

float AEnemyCharacter::GetHealth()
{
	return currentHealth;
}

void AEnemyCharacter::OnSpawn(FVector SpawnLocation)
{
	currentHealth = EnemyData->Health;
	bIsDead = false;
	GetCharacterMovement()->GravityScale = 1;
	SetActorLocation(SpawnLocation);
	if (AIController)
		AIController->SetIsDead(bIsDead);
}

void AEnemyCharacter::OnDespawn()
{
	bIsDead = true;
	if (AIController)
		AIController->SetIsDead(bIsDead);

	GetCharacterMovement()->GravityScale = 0;
	SetActorLocation(FVector(-10000,-10000,-10000));
	
}

bool AEnemyCharacter::CanSpawn()
{
	return bIsDead;
}


