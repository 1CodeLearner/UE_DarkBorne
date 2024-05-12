// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBItem.h"
#include "DBConsumable.generated.h"

class UDBEffect;
class ADBCharacter;
/**
 *
 */
UCLASS()
class DARKBORNE_API ADBConsumable : public ADBItem
{
	GENERATED_BODY()

public:
	TSubclassOf<UDBEffect> GetEffectClass() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UDBEffect> EffectClass;
};
