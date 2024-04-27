// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneDamage.generated.h"

class ADBPlayerController;
class ADBCharacter;

/**
 *
 */
UCLASS(Blueprintable)
class DARKBORNE_API UZoneDamage : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UZoneDamage();
	UFUNCTION(BlueprintCallable)
	void Initialize(ADBPlayerController* PC, float TotalTime, float DamageAmount);
	void UpdateTotalTime(float newTotalTime);
	UFUNCTION(BlueprintCallable)
	void StartTick();
	void StopTick();
	bool IsTicking() const;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	bool bIsTicking;
	float totalTime;
	float currTime;

	float damageAmt;

	ADBPlayerController* PlayerController;
	ADBCharacter* Character;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual UWorld* GetWorld() const override;
};
