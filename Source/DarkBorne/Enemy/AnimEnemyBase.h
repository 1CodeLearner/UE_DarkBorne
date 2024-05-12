// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Enemy/EnemyFSMBase.h"
#include "AnimEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UAnimEnemyBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state = EEnemyState::IDLE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isHitting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDeath = false;	

	UFUNCTION()
	void AnimNotify_End_Hit();
};
