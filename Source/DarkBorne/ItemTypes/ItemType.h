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

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE UMETA(DisplayName = "None"),
	WEAPON UMETA(DisplayName = "Weapon"),
	ARMOR UMETA(DisplayName = "Armor"),
	CONSUMABLE UMETA(DisplayName = "Consumable"),
	UTILITY UMETA(DisplayName = "Utility"),
	MAX UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
	NONE UMETA(DisplayName = "None"),
	COMMON UMETA(DisplayName = "Common"),
	RARE UMETA(DisplayName = "Rare"),
	EPIC UMETA(DisplayName = "Epic"),
	MAX UMETA(DisplayName = "Max")
};

USTRUCT(Blueprintable)
struct FRange
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float min;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float max;
};

/// <summary>
/// Defines base stat for any given item, excluding enhancements.
/// </summary>
USTRUCT(Blueprintable)
struct FEffect
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(Blueprintable)
struct FEffectHolder
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEffect> Effects;
};

UENUM(BlueprintType)
enum class EAttributeType : uint8 
{
	STRENGTH UMETA(DisplayName="Strength"),
	DEXTERITY UMETA(DisplayName="Dexterity")
};

USTRUCT(Blueprintable)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttributeType AttributeType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(Blueprintable)
struct FEnhancement : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;
};

USTRUCT(Blueprintable)
struct FDroppedItem
{
	GENERATED_USTRUCT_BODY()
public:
	FDroppedItem() = default;
	FDroppedItem(EItemType Type, float rate)
		:
		ItemType(Type),
		Probability(rate)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"), meta = (ClampMax = "1"))
	float Probability = 0.f;
};


/// <summary>
/// Defines the probability of drops and what types of items are dropped from monsters.
/// </summary>
USTRUCT(Blueprintable)
struct FDropRate : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	FDropRate()
	{
		Items.Add({ EItemType::WEAPON, 0.f });
		Items.Add({ EItemType::ARMOR, 0.f });
		Items.Add({ EItemType::CONSUMABLE, 0.f });
		Items.Add({ EItemType::UTILITY, 0.f });
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDroppedItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Amount;
};

/// <summary>
/// Item used in inventory, Do not modify anything in ItemSlot.
/// </summary>
USTRUCT(Blueprintable)
struct FItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FItem()
		: ItemSlot(nullptr)
	{
		//EffectHolder.Add({effect});
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPDA_ItemSlot* ItemSlot;

	UPROPERTY(BlueprintReadOnly)
	TArray<FEffect> Effects;

	UPROPERTY(BlueprintReadOnly)
	TArray<FEnhancement> Enhancements;
};

USTRUCT(Blueprintable)
struct FWeapon : public FItem
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(Blueprintable)
struct FArmor : public FItem
{
	GENERATED_USTRUCT_BODY()
}; 

USTRUCT(Blueprintable)
struct FConsumable : public FItem
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(Blueprintable)
struct FUtility : public FItem
{
	GENERATED_USTRUCT_BODY()
};