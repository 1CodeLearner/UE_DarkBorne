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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirH;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirV;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float jumpVelocity;

	UPROPERTY()
	class ADBRogueCharacter* RoguePlayer;

	UPROPERTY()
	class ADBWeapon_CloseRange* WeaponDagger;

	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	float pitchAngle;

	UPROPERTY(Replicated ,EditAnywhere, BlueprintReadWrite)
	bool isDeath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHitting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFalling = false;

public:
	UFUNCTION()
	void AnimNotify_Start_Damage();
	
	UFUNCTION()
	void AnimNotify_End_Damage();

	UFUNCTION()
	void AnimNotify_Start_Hit();

	UFUNCTION()
	void AnimNotify_End_Hit();
};
