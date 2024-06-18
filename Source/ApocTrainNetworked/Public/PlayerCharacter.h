// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"



UCLASS()
class APOCTRAINNETWORKED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

//protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool Interacted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerIndex;
protected:

	class AInteractableActor* groundedInteractable;

	class USphereComponent* InteractionTrigger;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DoMove(const FInputActionValue& Value);

	void DoLook(const FInputActionValue& Value);

	void DoDash(const FInputActionValue& Value);

	void DoInteract(const FInputActionValue& Value);
	void InteractReleased(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void Server_OnInteract(bool interacting);
	void Server_OnInteract_Implementation(bool interacting);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_OnInteract(bool interacting);
	void Multi_OnInteract_Implementation(bool interacting);

	UFUNCTION()
	void OnInteractionTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractionTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
