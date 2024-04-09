// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/DBItem.h"
#include "ItemType.generated.h"

class UPDA_ItemSlot;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	NONE UMETA(DisplayName = "None"),
	WEAPON UMETA(DisplayName = "Weapon"),
	ARMOR UMETA(DisplayName = "Armor"),
	CONSUMABLE UMETA(DisplayName = "Consumable"),
	UTILITY UMETA(DisplayName = "Utility")
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

USTRUCT(Blueprintable)
struct FItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FItem()
	: ItemClass(nullptr), ItemSlot(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADBItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPDA_ItemSlot* ItemSlot;
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