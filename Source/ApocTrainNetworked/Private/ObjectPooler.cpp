// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPooler.h"
#include "Poolable.h"

// Sets default values
AObjectPooler::AObjectPooler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectPooler::BeginPlay()
{
	Super::BeginPlay();
	CreateObjects();
}

void AObjectPooler::CreateObjects()
{
	for (int i = 0; i < TotalObjects; i++) {
		AActor* obj = GetWorld()->SpawnActor(ObjectType, new FVector(InitialPos.X,InitialPos.Y,InitialPos.Z), new FRotator(), FActorSpawnParameters());
		if (IPoolable* pooledObject = Cast<IPoolable>(obj)) {
			Pool.Enqueue(pooledObject);
		}
	}
}

// Called every frame
void AObjectPooler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectPooler::SpawnObject()
{
	if (Pool.IsEmpty()) {
		return;
	}
	if ((*Pool.Peek())->CanSpawn()) {
		IPoolable* pooledObject;
		Pool.Dequeue(pooledObject);
		pooledObject->OnSpawn();
		Pool.Enqueue(pooledObject);
	}
}

