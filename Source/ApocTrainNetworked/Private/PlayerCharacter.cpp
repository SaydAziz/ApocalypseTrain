// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"
#include "InteractableActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionTrigger"));
	SetReplicates(true);
	SetReplicateMovement(true);
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionTriggerBeginOverlap);
	InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnInteractionTriggerEndOverlap);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::DoInteract);
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &APlayerCharacter::InteractReleased);
	}

}

void APlayerCharacter::Server_OnInteract_Implementation(bool interacted)
{
	Multi_OnInteract(interacted);
}

void APlayerCharacter::Multi_OnInteract_Implementation(bool interacted)
{
	Interacted = interacted;
}

void APlayerCharacter::OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AInteractableActor* interactable = Cast<AInteractableActor>(OtherActor)) {
		if (groundedInteractable != NULL) {
			//still need to check in the interactable actor if a player is overlapping
			//groundedInteractable->OnPlayerOverlap(this);
		}
	}
}

void APlayerCharacter::OnInteractionTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AInteractableActor* interactable = Cast<AInteractableActor>(OtherActor)) {
		if (groundedInteractable != NULL) {
			//still need to check in the interactable actor if a player is overlapping
			//groundedInteractable->OnPlayerEndOverlap(this);
		}
	}
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

}

void APlayerCharacter::DoInteract(const FInputActionValue& Value)
{
	Server_OnInteract(true);
	if (groundedInteractable != NULL) {
		groundedInteractable->OnInteract(this);
	}
}

void APlayerCharacter::InteractReleased(const FInputActionValue& Value)
{
	Server_OnInteract(false);
}
