// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PotionEffect.h"
#include "PotionEffect_Healing.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UPotionEffect_Healing : public UPotionEffect
{
	GENERATED_BODY()
public:
	UPotionEffect_Healing();

protected:
	virtual void Initialize(ADBCharacter* Instigator, UItemObject* Item) override;

	virtual void Tick(float DeltaTime) override;

private:
	float prevHealAmount;
	float healTickRate;
	float nextHealTick;
};
