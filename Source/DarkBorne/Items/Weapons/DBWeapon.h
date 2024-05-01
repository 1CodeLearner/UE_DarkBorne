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
	ADBWeapon();

	//Delete this later
	UPROPERTY(EditAnywhere)
	int WeaponDamage = 10;
protected:
	//Delete this later
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Attack;
};
