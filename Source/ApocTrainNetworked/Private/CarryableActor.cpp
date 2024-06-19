// Fill out your copyright notice in the Description page of Project Settings.

#include "CarryableActor.h"
#include "PlayerCharacter.h"


void ACarryableActor::BeginPlay()
{
	Super::BeginPlay();
	PhysicsComponent = FindComponentByTag<UPrimitiveComponent>(FName("PhysicsComponent"));
	PhysicsComponent->OnComponentHit.AddDynamic(this, &ACarryableActor::OnPhysicsComponentHit);
}

void ACarryableActor::OnInteract(APlayerCharacter* player)
{
	if (!player->IsCarryingItem()) {
		player->Server_PickupItem(this);
		LastCarryingPlayer = player;
		carriedState = ECarriedState::Carried;
	}
}

bool ACarryableActor::CurrentlyInteractable()
{
	if (carriedState == ECarriedState::Idle) {
		return true;
	}
	return false;
}

void ACarryableActor::Server_OnPickedUp_Implementation(USceneComponent* carrier)
{
	Multi_OnPickedUp(carrier);
}

void ACarryableActor::Multi_OnPickedUp_Implementation(USceneComponent* carrier)
{
	if (carrier == NULL) {
		return;
	}
	PhysicsComponent->SetSimulatePhysics(false);
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	PhysicsComponent->AttachToComponent(carrier, rules);
	SetActorLocation(carrier->GetComponentLocation());
	MeshToHighlight->SetRenderCustomDepth(false);
}

void ACarryableActor::Server_DropObject_Implementation(FVector directionToLaunch)
{
	Multi_DropObject(directionToLaunch);
}

void ACarryableActor::Multi_DropObject_Implementation(FVector directionToLaunch)
{
	carriedState = ECarriedState::Dropped;
	PhysicsComponent->SetSimulatePhysics(true);
	PhysicsComponent->SetPhysicsLinearVelocity(FVector::Zero());
	PhysicsComponent->AddImpulse(directionToLaunch);
	wasDropped = true;
}

void ACarryableActor::OnPhysicsComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == NULL) {
		return;
	}
	if (OtherActor->ActorHasTag("Player")) {
		return;
	}
	if (wasDropped == true) {
		wasDropped = false;
		carriedState = ECarriedState::Idle;
		//this is where we call the pulse code
	}
}



