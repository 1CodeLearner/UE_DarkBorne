// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PotionEffect.h"
#include "PotionEffect_Protection.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UPotionEffect_Protection : public UPotionEffect
{
	GENERATED_BODY()
public:
	UPotionEffect_Protection();
	virtual void Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp) override;
	virtual void StartTick() override;
	virtual void StopTick() override;
protected:
	
};
