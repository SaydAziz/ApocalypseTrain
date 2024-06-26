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
	SetReplicateMovement(true);
	SetReplicates(true);
	if (carrier == NULL) {
		return;
	}
	PhysicsComponent->SetSimulatePhysics(false);
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	AttachToComponent(carrier, rules);
	SetActorLocation(carrier->GetComponentLocation());
	Multi_SetRenderDepth(false);
}

void ACarryableActor::Multi_SetRenderDepth_Implementation(bool renderdepth)
{
	MeshToHighlight->SetRenderCustomDepth(renderdepth);
}

void ACarryableActor::Server_DropObject_Implementation(FVector directionToLaunch)
{
	carriedState = ECarriedState::Dropped;
	PhysicsComponent->SetSimulatePhysics(true);
	PhysicsComponent->SetPhysicsLinearVelocity(FVector::Zero());
	PhysicsComponent->AddImpulse(directionToLaunch);
	wasDropped = true;
}

bool ACarryableActor::ShouldUpdateOverlappingPlayers()
{
	if (carriedState == ECarriedState::Carried) {
		return false;
	}
	return true;
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



