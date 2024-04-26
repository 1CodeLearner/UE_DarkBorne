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
UCLASS()
class DARKBORNE_API UZoneDamage : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UZoneDamage();
	void Initialize(ADBPlayerController* PC);
	void UpdateTotalTime(float newTotalTime);
	void StartTick();
	void StopTick();
protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	bool bIsTicking;
	float totalTime;
	float currTime;

	ADBPlayerController* PlayerController;
	ADBCharacter* Character;

	virtual UWorld* GetWorld() const override;
};
