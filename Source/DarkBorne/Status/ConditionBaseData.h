// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConditionBaseData.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UConditionBaseData : public UObject
{
	GENERATED_BODY()



public:
	FDelegateHandle handler;

	class UTexture2D* conditionImage;

	float delayTime;

	UFUNCTION()
	virtual void StartEvent();
	UFUNCTION()
	virtual void TickUpdate(float DeltaTime);
	UFUNCTION()
	virtual void EndEvent();

	
};
