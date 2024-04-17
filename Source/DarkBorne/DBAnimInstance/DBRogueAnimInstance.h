// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DBRogueAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UDBRogueAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool hasPistol;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirH;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float dirV;

	UPROPERTY()
	class ADBRogueCharacter* RoguePlayer;

	UPROPERTY()
	class ADBWeapon_CloseRange* WeaponDagger;

	UPROPERTY(BlueprintReadOnly)
	float pitchAngle;

	UPROPERTY(BlueprintReadOnly)
	bool isDeath = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShouldMove = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool HasWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool isAttacking;


public:
	UFUNCTION()
	void AnimNotify_Start_Damage();
	
	UFUNCTION()
	void AnimNotify_End_Damage();
};
