// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Status/ConditionBaseData.h"
#include "PoisonConditionData.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UPoisonConditionData : public UConditionBaseData
{
	GENERATED_BODY()



protected:
	virtual void StartEvent() override;
	virtual void TickUpdate(float DeltaTime) override;
	virtual void EndEvent() override;


	
	
};
