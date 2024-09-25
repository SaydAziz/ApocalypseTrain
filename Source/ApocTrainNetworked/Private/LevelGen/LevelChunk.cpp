// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGen/LevelChunk.h"

// Sets default values
ALevelChunk::ALevelChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelChunk::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	TSet<UActorComponent*> components = GetComponents();
	for (UActorComponent* Component : components)
	{
		if (Component->IsA(UStaticMeshComponent::StaticClass()))
		{
			UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(Component);
			if (mesh && mesh->ComponentHasTag("ChunkBase")) {
				chunkLength = (mesh->GetStaticMesh()->GetBounds().BoxExtent.Y * 2) * mesh->GetComponentScale().Y;
			}
		}
	}
}

// Called every frame
void ALevelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ALevelChunk::GetChunkLength()
{
	return chunkLength;
}

