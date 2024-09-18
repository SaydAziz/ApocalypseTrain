// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "CarryableActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "InputCoreTypes.h"
#include <Kismet/GameplayStatics.h>
#include <ATPlayerController.h>
#include "PlayerManager.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	characterMesh = FindComponentByClass<USkeletalMeshComponent>();
	CollisionCapsule = FindComponentByClass<UCapsuleComponent>();


	CurrentMovementState = EPlayerMovementState::standing;

	DashImpulseStrength = 2000.0f;
	DashCooldown = 1.0f;
	bCanDash = true;

	
}

void APlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource) {
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	carrySlot = FindComponentByTag<USceneComponent>("CarrySlot");

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	MaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CurrentHealth = MaxHealth;
	Server_SpawnDefaultWeapon();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	if (!bIsDead && bIsUsingMouse) {
		//get cursor in world space
		FVector HitLocation = GetHitResultUnderCursor();
		//rotate character
		RotateCharacterToLookAt(HitLocation);
	/*	if (Controller) {

			UE_LOG(LogTemp, Log, TEXT("Player index: %d Game Player Index: %d"), PlayerIndex, Cast<AATPlayerController>(Controller)->LocalPlayerIndex);
		}*/
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Dead: %d"), bIsDead));

	float CurrentSpeed = GetVelocity().Size();
	if (CurrentSpeed > GetCharacterMovement()->MaxWalkSpeed)
	{
		SetPlayerMovementState(EPlayerMovementState::dashing);
	}
	else if (CurrentSpeed > 0)
	{
		SetPlayerMovementState(EPlayerMovementState::walking);
	}
	else 
	{
		SetPlayerMovementState(EPlayerMovementState::standing);
	}

	//small hack to keep us from falling infinitly off the map for now. 
	//Bounds checking should probobly be a more universal thing, as we should use it to despawn other items
	if (playerManager && playerManager->IsOutOfBounds(GetActorLocation())) {
		Damage(99999999999);
	}

}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* playerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			playerSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoMove);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoLook);
		Input->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::DoDash);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAttack);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAttack);
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::InteractPressed);
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::InteractReleased);
	}

}

void APlayerCharacter::OnPlayerRegistered(APlayerManager* manager)
{
	playerManager = manager;
	//should also check if some global bool for the client that is Player 1 uses mouse is checked or something
	int localIndex = Cast<AATPlayerController>(Controller)->LocalPlayerIndex;
	if (localIndex == 0) {
		bIsUsingMouse = true;
	}
}

bool APlayerCharacter::IsCarryingItem()
{
	return CurrentActionState == EPlayerActionState::carrying;
}

bool APlayerCharacter::IsAttacking()
{
	return CurrentActionState == EPlayerActionState::attacking;
}

bool APlayerCharacter::IsFacingWall()
{
	FVector start = GetActorLocation();
	FVector forward = carrySlot->GetComponentLocation() - GetActorLocation();
	forward.Z = 0;
	FVector end = start + (forward * 1.2);
	FHitResult hit;

	if (GetWorld()) {
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(carriedObject);
		//QueryParams.AddIgnoredActor(CurrentWeapon);
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_WorldDynamic, QueryParams, FCollisionResponseParams());
		if (actorHit && hit.GetActor()) 
		{
			return true;
		}
	}
	return false;
}



void APlayerCharacter::Server_DropCarriedItem_Implementation()
{
	if (IsCarryingItem()) {
		if (carriedObject != NULL) {
			float upwardForce = 0.5f;
			if (IsFacingWall()) {
				carriedObject->SetActorLocation(GetActorLocation());
				carriedObject->Server_DropObject((this->GetActorForwardVector() * -1) * 0.2f * throwVelocity, GetActorLocation());
			}
			else {
				carriedObject->Server_DropObject(((this->GetActorForwardVector()) + FVector(0, 0, upwardForce)) * throwVelocity, GetActorLocation());
			}
		}
		SetPlayerActionState(EPlayerActionState::idle);
	}
}

void APlayerCharacter::Server_PickupItem_Implementation(ACarryableActor* itemToCarry)
{
	if (IsCarryingItem()) {
		return;
	}
	SetPlayerActionState(EPlayerActionState::carrying);
	carriedObject = itemToCarry;
	itemToCarry->Server_OnPickedUp(carrySlot);
	
}


void APlayerCharacter::Server_OnInteract_Implementation(bool interacted)
{
	Interacted = interacted;
}

void APlayerCharacter::Server_SpawnDefaultWeapon_Implementation()
{
	Server_EquipWeapon(Cast<AWeapon>(GetWorld()->SpawnActor(DefaultWeapon)));
}

void APlayerCharacter::DoMove(const FInputActionValue& Value)
{
	if (bIsDead) {
		return;
	}
	const FVector2D value = Value.Get<FVector2D>();

	if (Controller && ((value.X != 0) || (value.Y != 0)))
	{
		FVector ForwardDir = FVector(1, 0, 0);
		FVector RightDir = FVector(0, 1, 0);
		AddMovementInput(ForwardDir, value.Y);
		AddMovementInput(RightDir, value.X);
	}
}

void APlayerCharacter::DoLook(const FInputActionValue& Value)
{
	if (bIsUsingMouse) {
		return;
	}
	const FVector2D value = Value.Get<FVector2D>();
	if (Controller)
	{
		FVector RotVector = FVector(-value.Y, value.X, 0);
		FRotator RotDir = UKismetMathLibrary::MakeRotFromX(RotVector);
		FRotator NewRotation = FMath::Lerp(GetControlRotation(), RotDir, 0.2f);
		Controller->SetControlRotation(NewRotation);
	}
}

void APlayerCharacter::RotateCharacterToLookAt( FVector TargetPosition)
{
	// Get the current character location
	FVector CharacterLocation = GetActorLocation();

	TargetPosition = FVector(TargetPosition.X, TargetPosition.Y, CharacterLocation.Z);
	// Calculate the direction to the target position 
	FVector Direction = TargetPosition - CharacterLocation;

	if (!Direction.IsNearlyZero())
	{
		// Get the target rotation from the direction vector
		FRotator TargetRotation = Direction.Rotation();

		// Smoothly interpolate the rotation for smoother turning
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		if (Controller) {
			Controller->SetControlRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
		}
	}
}

void APlayerCharacter::DespawnPlayer()
{
	CurrentHealth = 0;
	currentRespawnTime = respawnTime;
	bIsDead = true;
	StopAttacking();
	SetActorLocation(playerManager->GetPlayerDeathPos());
	if (HasAuthority()) {
		GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this, &APlayerCharacter::RespawnPlayer, 1, false);
	}
}

void APlayerCharacter::RespawnPlayer()
{
	if (HasAuthority()) {
		currentRespawnTime -= 1;
		if (currentRespawnTime > 0) {
			GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, this, &APlayerCharacter::RespawnPlayer, 1, false);
			return;
		}
		else {
			CurrentHealth = MaxHealth;
			bIsDead = false;
			SetActorLocation(playerManager->GetPlayerSpawnPoint());
		}
	}
}

void APlayerCharacter::Damage(float damageToTake)
{
	if (bIsDead) {
		return;
	}
	if (HasAuthority()) {
		GetCharacterMovement()->MaxWalkSpeed = InjuredMoveSpeed;
		GetWorld()->GetTimerManager().SetTimer(damageSlowTimerHandle, this, &APlayerCharacter::ResetMovementSpeed, DamageSlowTime, false);
		CurrentHealth -= damageToTake;
		if (CurrentHealth <= 0) {
			DespawnPlayer();
		}
	}
}

float APlayerCharacter::GetHealth()
{
	return CurrentHealth;
}

FVector APlayerCharacter::GetHitResultUnderCursor()
{
	if (Controller && IsLocallyControlled())
	{
		FHitResult HitResult;
		if (Cast<APlayerController>(Controller)->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
		{
			return HitResult.Location;
		}
	}
	return FVector(0,0,0);
}

void APlayerCharacter::DoDash(const FInputActionValue& Value)
{
	if (bCanDash)
	{
		FVector DashDir = GetVelocity().GetSafeNormal();
		FVector Impulse = DashDir * DashImpulseStrength;

		GetCharacterMovement()->Launch(Impulse);

		bCanDash = false;
		GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerCharacter::ResetDash, DashCooldown, false);

		if (!HasAuthority()) 
		{
			Server_DoDash(Impulse);
		}
	}
}

void APlayerCharacter::Server_DoDash_Implementation(FVector Impulse)
{
	GetCharacterMovement()->Launch(Impulse);
}

void APlayerCharacter::StartAttack(const FInputActionValue& Value)
{
	if (bIsDead) {
		return;
	}
	if (!IsCarryingItem()) {
		SetPlayerActionState(EPlayerActionState::attacking);
		EquippedWeapon->StartAttack();
	}
}

void APlayerCharacter::StopAttack(const FInputActionValue& Value)
{
	StopAttacking();
}

void APlayerCharacter::StopAttacking()
{
	if (IsAttacking()) {
		EquippedWeapon->StopAttack();
		if (!IsCarryingItem()) {
			SetPlayerActionState(EPlayerActionState::idle);
		}
	}
}

void APlayerCharacter::ResetDash()
{
	bCanDash = true;
}

void APlayerCharacter::InteractPressed(const FInputActionValue& Value)
{
	if (WeaponOnGround != NULL)
	{
		Server_EquipWeapon(WeaponOnGround);
		WeaponOnGround = NULL;
	}
	else
	{
		Server_OnInteract(true);
		Server_DropCarriedItem();
	}

}

void APlayerCharacter::InteractReleased(const FInputActionValue& Value)
{
	Server_OnInteract(false);
}

void APlayerCharacter::ResetMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWeapon::StaticClass()) )
	{
		if (OtherActor != EquippedWeapon)
		{
			WeaponOnGround = Cast<AWeapon>(OtherActor);
			if (WeaponOnGround)
			{
				WeaponOnGround->Highlight(true);
			}
		}
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == WeaponOnGround)
	{
		WeaponOnGround->Highlight(false);
		WeaponOnGround = NULL;
	}
}

void APlayerCharacter::Server_EquipWeapon_Implementation(AWeapon* Weapon)
{

	if (Weapon->GetAttachParentActor() == nullptr)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}

		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);

		EquippedWeapon = Weapon;
		EquippedWeapon->SetActorLocation(characterMesh->GetSocketTransform("WeaponSocket").GetLocation());
		EquippedWeapon->AttachToComponent(characterMesh, AttachmentRules, "WeaponSocket");
		EquippedWeapon->Equip();
		EquippedWeapon->SetOwner(this);
	}
}

int APlayerCharacter::GetPlayerIndex()
{
	//return Cast<AATPlayerController>(Controller)->PlayerIndex;
	return PlayerIndex;
}

//void APlayerCharacter::SetPlayerIndex(int index)
//{
//	Cast<AATPlayerController>(Controller)->PlayerIndex = index;
//}

void APlayerCharacter::SetPlayerMovementState(EPlayerMovementState NewMovementState)
{
	if (CurrentMovementState != NewMovementState)
	{
		CurrentMovementState = NewMovementState;
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("Switched to: %d"), CurrentMovementState));

		//Add transition logic here
	}

}

EPlayerMovementState APlayerCharacter::GetPlayerMovementState() const
{
	return CurrentMovementState;
}

void APlayerCharacter::SetPlayerActionState(EPlayerActionState NewActionState)
{
	if (CurrentActionState != NewActionState) {
		switch (NewActionState) {
			case EPlayerActionState::idle:
				if (CurrentActionState == EPlayerActionState::carrying) {
				//AttachWeapon(); //this is for putting the weapon back in the players hands from their back
				}
				break;
			case EPlayerActionState::attacking:
				break;
			case EPlayerActionState::carrying:
				//HolsterWeapon(); //this is for putting the weapon on the players back while they are holding something
				EquippedWeapon->StopAttack();
				break;
		}
		CurrentActionState = NewActionState;
	}
}


void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, EquippedWeapon);
	DOREPLIFETIME(APlayerCharacter, PlayerIndex);
	DOREPLIFETIME(APlayerCharacter, bIsUsingMouse);
}