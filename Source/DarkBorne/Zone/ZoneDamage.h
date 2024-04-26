// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneDamage.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UZoneDamage : public UObject, public FTickableGameObject
{
	GENERATED_BODY()


protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
