// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "FlashComponent.h"
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
	GetCharacterMovement()->MaxWalkSpeed = EnemyData->Speed;
	currentHealth = EnemyData->Health;
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
		//despawn enemy
		currentHealth = 0;
	}
}

float AEnemyCharacter::GetHealth()
{
	return currentHealth;
}

