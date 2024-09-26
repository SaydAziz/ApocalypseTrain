// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FuelComponent.generated.h"



UENUM()
enum class EFuelState : uint8 {
	normal UMETA(DIsplayName = "Normal"), critical UMETA(DIsplayName = "Critical")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class APOCTRAINNETWORKED_API UFuelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFuelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	void OnDepositBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bBurning;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite)
	float CurrentFuel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxFuel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CriticalLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BurnRate;

	bool IsInsideDeposit(FVector location);

	bool IsFuelCritical();
	bool HasFuel();

	UFUNCTION(Server, Unreliable)
	void Server_AddFuel(class AFuel* fuel);
	void Server_AddFuel_Implementation(class AFuel* fuel);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	void BurnFuel(float DeltaTime);
	bool IsFull();
	class UBoxComponent* DepositTrigger;
	class UStaticMeshComponent* MeshToHighlight;
	class APlayerManager* PlayerManager;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerWithFuelOverlappingUpdated)
	bool bPlayerWithFuelOverlapping;

	UFUNCTION()
	void OnRep_PlayerWithFuelOverlappingUpdated();

	UFUNCTION(Server, Unreliable)
	void Server_PlayersOverlapping(bool overlapping);
	virtual void Server_PlayersOverlapping_Implementation(bool overlapping);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayersOverlapping(bool overlapping);
	virtual void Multi_PlayersOverlapping_Implementation(bool overlapping);

};
