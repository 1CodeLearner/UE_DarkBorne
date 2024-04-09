// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBItem.h"
#include "DBWeapon.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API ADBWeapon : public ADBItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int WeaponDamage = 10;

protected:
	
protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Attack;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TArray<UAnimMontage*> AnimMontages;
};
