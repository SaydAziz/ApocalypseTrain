// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "CarryableActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	characterMesh = FindComponentByClass<USkeletalMeshComponent>();

	CurrentMovementState = EPlayerMovementState::standing;

	DashImpulseStrength = 2000.0f;
	DashCooldown = 1.0f;
	bCanDash = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	carrySlot = FindComponentByTag<USceneComponent>("CarrySlot");
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, FString::Printf(TEXT("Switched to: %d"), CurrentState));

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
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::InteractPressed);
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::InteractReleased);
	}

}

bool APlayerCharacter::IsCarryingItem()
{
	return CarryingItem;
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
		/*if (actorHit && hit.GetActor()) {
			if (AEnemyCharacter* enemy = Cast<AEnemyCharacter>(hit.GetActor())) {
				return false;
			}
			if (AObstacle* obstacle = Cast<AObstacle>(hit.GetActor())) {
				return false;
			}
			return true;
		}*/
	}
	return false;
}

void APlayerCharacter::Server_DropCarriedItem_Implementation()
{
	if (CarryingItem) {
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
		CarryingItem = false;
		//AttachWeapon();
	}
}

void APlayerCharacter::Server_PickupItem_Implementation(ACarryableActor* itemToCarry)
{
	if (CarryingItem) {
		return;
	}
	CarryingItem = true;
	carriedObject = itemToCarry;
	//HolsterWeapon();
	itemToCarry->Server_OnPickedUp(carrySlot);
	//ShootReleased();
}


void APlayerCharacter::Server_OnInteract_Implementation(bool interacted)
{
	Interacted = interacted;
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
	const FVector2D value = Value.Get<FVector2D>();
	if (Controller)
	{
		FVector RotVector = FVector(-value.Y, value.X, 0);
		FRotator RotDir = UKismetMathLibrary::MakeRotFromX(RotVector);
		FRotator NewRotation = FMath::Lerp(GetControlRotation(), RotDir, 0.2f);
		Controller->SetControlRotation(NewRotation);
	}
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

	}
}


void APlayerCharacter::ResetDash()
{
	bCanDash = true;
}

void APlayerCharacter::InteractPressed(const FInputActionValue& Value)
{
	Server_OnInteract(true);
	Server_DropCarriedItem();
}

void APlayerCharacter::InteractReleased(const FInputActionValue& Value)
{
	Server_OnInteract(false);
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
