// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/DBItem.h"
#include "ItemType.generated.h"


class UPDA_ItemSlot;

UENUM()
enum class ESlotType : uint8
{
	WEAPON UMETA(DisplayName = "Weapon"),
	UTILITY UMETA(DisplayName = "Utility"),
	CONSUMABLE UMETA(DisplayName = "Consumable"),
	HEAD UMETA(DisplayName = "Head"),
	UPPERWEAR UMETA(DisplayName = "UpperWear"),
	BOTTOMWEAR UMETA(DisplayName = "BottomWear"),
	GLOVES UMETA(DisplayName = "Gloves"),
	BOOTS UMETA(DisplayName = "Boots"),
	NONE UMETA(Displayname = "None")
};

USTRUCT()
struct FDimension
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float X;
	UPROPERTY(EditAnywhere)
	float Y;
};

USTRUCT()
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADBItem> ItemClass;

	UPROPERTY(EditAnywhere)
	UPDA_ItemSlot* ItemSlot;
};

USTRUCT()
struct FWeapon : public FItem
{
	GENERATED_BODY()
};

USTRUCT()
struct FArmor : public FItem
{
	GENERATED_BODY()
};

USTRUCT()
struct FConsumable : public FItem
{
	GENERATED_BODY()
};

USTRUCT()
struct FUtility : public FItem
{
	GENERATED_BODY()
};