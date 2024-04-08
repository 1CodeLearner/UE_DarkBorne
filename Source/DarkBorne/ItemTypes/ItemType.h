// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/DBItem.h"
#include "ItemType.generated.h"

class UPDA_ItemSlot;

UENUM()
enum class EItemType : uint8
{
	WEAPON UMETA(DisplayName = "Weapon"),
	ARMOR UMETA(DisplayName = "Armor"),
	CONSUMABLE UMETA(DisplayName = "Consumable"),
	UTILITY UMETA(DisplayName = "Utility")
};

USTRUCT()
struct FDroppedItem
{
	GENERATED_BODY()
public:
	FDroppedItem() = default;
	FDroppedItem(EItemType Type, float rate)
		: 
		ItemType(Type), 
		Probability(rate)
	{}

	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"), meta = (ClampMax = "1"))
	float Probability = 0.f;
};

USTRUCT()
struct FDropRate : public FTableRowBase
{
	GENERATED_BODY()
public:

	FDropRate()
	{
		Items.Add({ EItemType::WEAPON, 0.f });
		Items.Add({ EItemType::ARMOR, 0.f });
		Items.Add({ EItemType::CONSUMABLE, 0.f });
		Items.Add({ EItemType::UTILITY, 0.f });
	}
	
	UPROPERTY(EditAnywhere)
	TArray<FDroppedItem> Items;
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