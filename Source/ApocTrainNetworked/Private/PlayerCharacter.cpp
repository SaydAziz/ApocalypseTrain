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

	CurrentHealth = 100.0f;
	Server_SpawnDefaultWeapon();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	//if a gamepad is not connected, make sure to rotate controller towards the mouse position
	if (!IsGamepadConnected()) {
		//get cursor in world space
		FVector HitLocation = GetHitResultUnderCursorExample();
		//rotate character
		RotateCharacterToLookAt(HitLocation);
	}

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

}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, EquippedWeapon);
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
		//Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DoLook);

		Input->BindAction(DashAction, ETriggerEvent::Started, this, &APlayerCharacter::DoDash);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAttack);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAttack);
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::InteractPressed);
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::InteractReleased);
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

void APlayerCharacter::Damage(float damageToTake)
{
	CurrentHealth -= damageToTake;
	if (CurrentHealth <= 0) {
		CurrentHealth = 0;
		//Implement Death mechanics here
	}
}

float APlayerCharacter::GetHealth()
{
	return CurrentHealth;
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
	FVector2D value = Value.Get<FVector2D>();
	
	//if there is a gamepad connected, use gamepad rotation
	if (IsGamepadConnected() && Controller)
	{
		FVector RotVector = FVector(-value.Y, value.X, 0);
		FRotator RotDir = UKismetMathLibrary::MakeRotFromX(RotVector);
		FRotator NewRotation = FMath::Lerp(GetControlRotation(), RotDir, 0.2f);
		Controller->SetControlRotation(NewRotation);
	}
	else {
		FVector HitLocation = GetHitResultUnderCursorExample();
		RotateCharacterToLookAt(HitLocation);
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

		Controller->SetControlRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	}
}

FVector APlayerCharacter::GetHitResultUnderCursorExample()
{
	if (Controller)
	{
		FHitResult HitResult;
		if (Cast<APlayerController>(Controller)->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
		{
			return HitResult.Location;
		}
	}
	return FVector(0,0,0);
}

bool APlayerCharacter::IsGamepadConnected()
{
	if (FSlateApplication::IsInitialized())
	{
		return FSlateApplication::Get().IsGamepadAttached();
	}
	return false;
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
	if (!IsCarryingItem()) {
		SetPlayerActionState(EPlayerActionState::attacking);
		EquippedWeapon->StartAttack();
	}
}

void APlayerCharacter::StopAttack(const FInputActionValue& Value)
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
