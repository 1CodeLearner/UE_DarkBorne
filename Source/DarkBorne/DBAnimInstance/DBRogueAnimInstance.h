// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Status/CharacterStatusAnimInterface.h"
#include "DBRogueAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UDBRogueAnimInstance : public UAnimInstance, public ICharacterStatusAnimInterface
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
	bool isCastingShift = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHitting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFalling = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool isDoubleJumping = false;

public:
	UFUNCTION()
	void AnimNotify_Start_Damage();
	UFUNCTION()
	void AnimNotify_End_Damage();

	UFUNCTION()
	void AnimNotify_Start_Hit();
	UFUNCTION()
	void AnimNotify_End_Hit();
	
	UFUNCTION()
	void AnimNotify_Jump_End();
	UFUNCTION()
	void AnimNotify_DoubleJumpStart();
	UFUNCTION()
	void AnimNotify_DoubleJumpEnd();

	UFUNCTION()
	void AnimNotify_Start_Shift();
	UFUNCTION()
	void AnimNotify_End_Shift();
};
