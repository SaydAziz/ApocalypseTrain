// Fill out your copyright notice in the Description page of Project Settings.


#include "ATPlayerController.h"

void AATPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (IsLocalController()) {
        LocalPlayerIndex = GetLocalPlayerIndex();
        UE_LOG(LogTemp, Log, TEXT("CONTROLLER Player Index: %d"), LocalPlayerIndex);
    }
}

int AATPlayerController::GetLocalPlayerIndex()
{
    if (IsLocalController()) {
        ULocalPlayer* LocalPlayer = GetLocalPlayer();
        if (LocalPlayer)
        {
            UGameInstance* GameInstance = GetWorld()->GetGameInstance();
            if (GameInstance)
            {
                const TArray<ULocalPlayer*>& LocalPlayers = GameInstance->GetLocalPlayers();

                int32 PIndex = LocalPlayers.IndexOfByKey(LocalPlayer);

                // Add logging
                UE_LOG(LogTemp, Warning, TEXT("Found LocalPlayer: %s"), *LocalPlayer->GetName());
                UE_LOG(LogTemp, Warning, TEXT("LocalPlayers Count: %d"), LocalPlayers.Num());
                UE_LOG(LogTemp, Warning, TEXT("LocalPlayer Index: %d"), PIndex);

                return PIndex;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("GameInstance is null"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player is not a ULocalPlayer"));
        }
    }
    return -1;
}
