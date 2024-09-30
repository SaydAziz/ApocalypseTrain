// Fill out your copyright notice in the Description page of Project Settings.

#include "Entities/CarryableActor.h"
#include "Core/PlayerCharacter.h"
#include "ATComponents/PulseComponent.h"


ACarryableActor::ACarryableActor()
{
	pulseComponent = CreateDefaultSubobject<UPulseComponent>(TEXT("PulseComponent"));
}

void ACarryableActor::BeginPlay()
{
	Super::BeginPlay();
	PhysicsComponent = FindComponentByTag<UPrimitiveComponent>(FName("PhysicsComponent"));
	PhysicsComponent->OnComponentHit.AddDynamic(this, &ACarryableActor::OnPhysicsComponentHit);
}

void ACarryableActor::OnInteract(APlayerCharacter* player)
{
	if (!player->IsCarryingItem()) {
		carriedState = ECarriedState::Carried;
		player->Server_PickupItem(this);
		LastCarryingPlayer = player;
	}
}

bool ACarryableActor::CurrentlyInteractable()
{
	return carriedState == ECarriedState::Idle;
}

bool ACarryableActor::IsCarried()
{
	return carriedState == ECarriedState::Carried;
}

void ACarryableActor::Server_OnPickedUp_Implementation(USkeletalMeshComponent* carrier)
{
	SetReplicateMovement(true);
	SetReplicates(true);
	if (carrier == NULL) {
		return;
	}
	PhysicsComponent->SetSimulatePhysics(false);
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	
	SetActorLocation(carrier->GetSocketTransform("CarrySocket").GetLocation());
	AttachToComponent(carrier, rules, "CarrySocket");

	Multi_SetRenderDepth(false);
}

void ACarryableActor::Multi_SetRenderDepth_Implementation(bool renderdepth)
{
	MeshToHighlight->SetRenderCustomDepth(renderdepth);
}

void ACarryableActor::Server_DropObject_Implementation(FVector directionToLaunch, FVector dropperLocation)
{
	carriedState = ECarriedState::Dropped;
	PhysicsComponent->SetSimulatePhysics(true);
	//PhysicsComponent->SetPhysicsLinearVelocity(LastCarryingPlayer->GetVelocity());
	PhysicsComponent->SetPhysicsLinearVelocity(FVector(0,0,0));
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
		if (pulseComponent) {
			pulseComponent->Pulse();
		}
	}
}



