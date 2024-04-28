// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneNode.generated.h"

UCLASS()
class DARKBORNE_API AZoneNode : public AActor
{
	GENERATED_BODY()

public:
	AZoneNode();
public:

protected:
	virtual void BeginPlay() override;

};
