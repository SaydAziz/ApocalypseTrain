// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyCharacter.h"
#include "Core/PlayerCharacter.h"
#include "ATComponents/FlashComponent.h"
#include "Enemies/EnemyAIController.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "ATComponents/DamageComponent.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageComponent = CreateDefaultSubobject<UDamageComponent>("Damage Component");
	DamageComponent->SetIsReplicated(true);
	DamageComponent->OnDamageTaken.AddDynamic(this, &AEnemyCharacter::TakeDamage);
	DamageComponent->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDespawn);

	FlashComponent = CreateDefaultSubobject<UFlashComponent>("Flash Component");

	AttackBox = CreateDefaultSubobject<UBoxComponent>("Attack Box");
	AttackBox->SetupAttachment(RootComponent);

	BloodSplatComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BloodSplatterVFX"));
	BloodSplatComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	InitializeEnemy();

	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);
	AttackBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapEnd);

	if (BloodSplatSystem) BloodSplatComp->SetAsset(BloodSplatSystem);
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
	DamageComponent->MaxHealth = EnemyData->Health;
	DamageComponent->Reset();
	bCanAttack = true;
	CurrentState = EEnemyState::idle;
	if (!bIsNotPooled) {
		OnDespawn();
	}
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

void AEnemyCharacter::Server_OnDamaged_Implementation(float damageTaken) {
	if (HasAuthority()) {
		OnDamaged(damageTaken);
		BloodSplatComp->Activate();
	}
	Multi_OnDamaged(damageTaken);
}

void AEnemyCharacter::Multi_OnDamaged_Implementation(float damageTaken) {
	if (!HasAuthority()) {
		OnDamaged(damageTaken);
		BloodSplatComp->Activate();
	}
}

void AEnemyCharacter::TakeDamage(float damageToTake)
{
	if (bIsDead) {
		return;
	}
	if (FlashComponent) {
		FlashComponent->Flash();
	}
	if (HasAuthority()) {
		Server_OnDamaged(DamageComponent->GetHealth());
	}
}

void AEnemyCharacter::OnSpawn(FVector SpawnLocation)
{
	DamageComponent->MaxHealth = EnemyData->Health;
	DamageComponent->Reset();
	bIsDead = false;
	GetCharacterMovement()->GravityScale = 1;
	SetActorLocation(SpawnLocation);
	if (AIController)
		AIController->SetIsDead(bIsDead);
}

void AEnemyCharacter::OnDespawn()
{
	bIsDead = true;
	if (EnemyData->DeathVFX) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EnemyData->DeathVFX, GetActorLocation() + DeathVFXPositionOffset, GetActorForwardVector().Rotation(),FVector(DeathVFXScale, DeathVFXScale, DeathVFXScale));
	}
	DamageComponent->SetNegateDamageTrue();
	if (AIController)
		AIController->SetIsDead(bIsDead);
	GetCharacterMovement()->GravityScale = 0;
	SetActorLocation(FVector(-10000,-10000,-10000));
}

bool AEnemyCharacter::CanSpawn()
{
	return bIsDead;
}

void AEnemyCharacter::IncreaseEnemyDifficulty()
{

}

float AEnemyCharacter::GetAttackRadius()
{
	return EnemyData->AttackRadius;
}

void AEnemyCharacter::ExecuteMeleeAttack()
{
	//FString StateString = FString::Printf(TEXT("%d"), static_cast<int32>(CurrentState));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, StateString);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("ATTACKING!"));
	bCanAttack = false;
	SetEnemyState(EEnemyState::attacking);
	GetWorldTimerManager().SetTimer(AttackRateTimerHandle, this, &AEnemyCharacter::ResetAttack, EnemyData->AttackRate, false);
}

bool AEnemyCharacter::CanAttack()
{
	return bCanAttack;
}

void AEnemyCharacter::EnableAttackBox()
{
	//AttackBox->SetActive(true);
	//GetWorldTimerManager().SetTimer(AttackBoxTimerHandle, this, &AEnemyCharacter::DisableAttackBox, 0.5f, false);
	TArray<AActor*> OverlappingActors;
	AttackBox->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->IsA(APlayerCharacter::StaticClass()))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("HIT!!!!!!!!!!!!!!!!!!!!!!!!!"));
			Cast<APlayerCharacter>(OverlappingActor)->GetComponentByClass<UDamageComponent>()->Damage(EnemyData->Damage);
			return;
		}
	}
}


void AEnemyCharacter::DisableAttackBox()
{
	GetWorldTimerManager().ClearTimer(AttackBoxTimerHandle);
	AttackBox->SetActive(false);
}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyCharacter, CurrentState);
}

void AEnemyCharacter::ResetAttack()
{
	GetWorldTimerManager().ClearTimer(AttackRateTimerHandle);
	bCanAttack = true;
}

void AEnemyCharacter::SetEnemyState(EEnemyState NewState)
{
	if (HasAuthority())
	{
		if (CurrentState != NewState)
		{
			FString StateString = FString::Printf(TEXT("%d"), static_cast<int32>(NewState));
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, StateString);
			CurrentState = NewState;
		}
	}
}

EEnemyState AEnemyCharacter::GetEnemyState() const
{
	return CurrentState;
}

void AEnemyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("KILLBARRIER")) {
		DamageComponent->Damage(9999999999999);
	}
}

void AEnemyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


