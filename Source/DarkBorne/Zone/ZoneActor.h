// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneActor.generated.h"


USTRUCT()
struct FTransformZone 
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Loc;
	UPROPERTY()
	FVector Scale;
};

class AZoneNode;
class UCapsuleComponent;
class ADBPlayerController;
class UZoneDamage;

UCLASS()
class DARKBORNE_API AZoneActor : public AActor
{
	GENERATED_BODY()

public:
	AZoneActor();
	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCapsuleComponent> CapsuleComp;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	TArray<AZoneNode*> Nodes;

private:

	int index;
	bool CanMove() const;
	void StartMove();
	void UpdateMovement(float DeltaTime);

	FVector prevLoc;
	FVector nextLoc;

	FVector currScale;
	FVector nextScale;
	FVector diffScale;

	UPROPERTY(ReplicatedUsing = "OnRep_TransformZone")
	FTransformZone TransformZone;
	UFUNCTION()
	void OnRep_TransformZone();


	float currMoveTime;
	float maxMoveTime;
	
	bool bMove;

	float currWaitTime;
	float maxWaitTime;

	UPROPERTY()
	TMap<ADBPlayerController*,bool> playerOverlapped;
	UPROPERTY()
	TMap<ADBPlayerController*,UZoneDamage*> playerDamaged;

	void HandlePlayersOverlapped();

	UFUNCTION()
	void OnPlayerUpdate(ADBPlayerController* Player, bool bExit);

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
