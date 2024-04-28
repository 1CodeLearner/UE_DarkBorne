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

USTRUCT()
struct FZonePhase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float DisplayTime;
	UPROPERTY(EditAnywhere)
	float ShrinkTime;
	UPROPERTY(EditAnywhere)
	float Damage;
};

USTRUCT()
struct FZoneSetting : public FTableRowBase
{
	GENERATED_BODY()

public:
	float GetTotalTime();
	const TArray<FZonePhase>& GetPhases() const;
protected:
	float TotalTime = 0.f;

	UPROPERTY(EditAnywhere)
	TArray<FZonePhase> Phases;

private:
	bool bInitialized = false;
};

class AZoneNode;
class UCapsuleComponent;
class ADBPlayerController;
class UZoneDamage;
class ADBCharacter;

static TAutoConsoleVariable<bool> cVarDisplayZoneDebugMsg(TEXT("su.DisplayZoneDebugMsg"), false, TEXT("Display Zone Actor Debug info"), ECVF_Cheat);

UCLASS()
class DARKBORNE_API AZoneActor : public AActor
{
	GENERATED_BODY()

public:
	AZoneActor();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UCapsuleComponent> CapsuleComp;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	UDataTable* DT_ZoneSetting;

private:
	UPROPERTY()
	TMap<ADBPlayerController*, ADBCharacter*> ActiveCharacters;
	UPROPERTY()
	TMap<ADBPlayerController*, bool> playerOverlapped;
	UPROPERTY()
	TMap<ADBPlayerController*, UZoneDamage*> playerDamaged;


	//used to check if a given player is within the zone
	FVector currLoc;
	bool IsWithinZone(FVector Other) const;
	float GetDistanceFromOrigin(FVector OtherLocation) const;
	float GetZoneRadius() const;

	bool CheckSizes() const;
	
	bool bIsFirstStarting;
	FZoneSetting* ZoneSetting;

	UPROPERTY()
	AZoneNode* StartingNode;
	UPROPERTY()
	TArray<AZoneNode*> Nodes;
	
	int PhaseCount;
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

	void UpdatePlayerOverlapped();
	void UpdatePlayerDamaged();

	float currMoveTime;
	float maxMoveTime;

	bool bMove;

	float currWaitTime;
	float maxWaitTime;


	UFUNCTION()
	void OnPlayerUpdate(ADBPlayerController* Player, bool bExit);
	UFUNCTION()
	void OnGameEnd(ADBPlayerController* PlayerWon);
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
};
