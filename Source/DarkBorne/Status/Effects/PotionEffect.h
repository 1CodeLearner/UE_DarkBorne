// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBEffect.h"
#include "PotionEffect.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UPotionEffect : public UDBEffect
{
	GENERATED_BODY()
public:
	UPotionEffect();
protected:
	float Amount;
};
