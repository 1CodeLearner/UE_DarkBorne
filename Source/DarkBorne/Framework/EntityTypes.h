// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntityTypes.generated.h"


UENUM(BlueprintType)
enum class EEntityType : uint8
{
	PLAYER UMETA(DisplayName="Player"),
	ENEMY UMETA(DisplayName="Enemy"),
	NONE UMETA(DisplayName="None"),
	MAX UMETA(Displayname="Max")
};