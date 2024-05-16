 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/AnimEnemyBase.h"
#include "../Enemy/MorigeshEnemyFSM.h"
#include "AnimMorigeshEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UAnimMorigeshEnemy : public UAnimEnemyBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMorigeshState morigeshState;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMorigashAttackType morigeshAttackType;

};
