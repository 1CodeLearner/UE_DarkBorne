// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnchantmentTypes.h"
#include "ItemType.generated.h"

class UPDA_ItemSlot;
class ADBItem;

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
	EItemType ItemType = EItemType::NONE;

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
	int Amount = 0;
};

USTRUCT(Blueprintable)
struct FSlotHolder
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UMaterialInterface* DisplayMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText LoreText;

	//인벤토리 안에 아이탬 옮기는 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	USoundBase* InventorySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	ESlotType SlotType;

	//아이템이 인벤토리 차지하는 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FDimension SlotDimension;

	//아이탬 장착할때 내는 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TArray<FRarity> Rarities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<ADBItem> ItemClass;
};

/// <summary>
/// Item used in inventory, Do not modify anything in SlotHolder during runtime.
/// </summary>
USTRUCT(Blueprintable)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPDA_ItemSlot* ItemSlot;
public:
	void Initialize();
		
	UPROPERTY(BlueprintReadOnly)
	FSlotHolder SlotHolder;

	UPROPERTY(BlueprintReadOnly)
	TArray<FRarity> Rarities;

	UPROPERTY(BlueprintReadOnly)
	FDarkBorneStats Enchantments;
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