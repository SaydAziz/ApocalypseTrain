// Fill out your copyright notice in the Description page of Project Settings.


#include "CarryableActor.h"

void ACarryableActor::DropObject(FVector directionToLaunch)
{
}

bool ACarryableActor::CurrentlyInteractable()
{
	if (carriedState == ECarriedState::Idle) {
		return true;
	}
	return false;
}
