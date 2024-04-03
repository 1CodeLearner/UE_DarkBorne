// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlahComponent.h"
#include "ChildTestComponent.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UChildTestComponent : public UBlahComponent
{
	GENERATED_BODY()
	
public:
	virtual void TestFunc() override;
};
