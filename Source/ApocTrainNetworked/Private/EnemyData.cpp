// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyData.h"

void UEnemyData::IncreaseDifficulty()
{
	Health += 10;
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, FString::Printf(TEXT("HEalth %f"), Health));
}
