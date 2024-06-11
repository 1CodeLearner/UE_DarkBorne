// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnchantmentTypes.h"
#include "ItemType.generated.h"

class UPDA_ItemSlot;
class ADBItem;
class UTexture2D;

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
struct FDimension
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float X = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Y = 0.f;
};

USTRUCT(Blueprintable)
struct FSlotHolder
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Id;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInterface* DisplayMaterial;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* IconTexture; 

	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FText LoreText;

	UPROPERTY(BlueprintReadOnly)
	FText EffectText;

	//인벤토리 안에 아이탬 옮기는 소리
	UPROPERTY(BlueprintReadOnly)
	USoundBase* InventorySound;

	UPROPERTY(BlueprintReadOnly)
	ESlotType SlotType;

	//아이템이 인벤토리 차지하는 사이즈
	UPROPERTY(BlueprintReadOnly)
	FDimension SlotDimension;

	//아이탬 장착할때 내는 소리
	UPROPERTY(BlueprintReadOnly)
	USoundBase* EquipSound;

	//아이탬 떨어트릴때 내는 소리
	UPROPERTY(BlueprintReadOnly)
	USoundBase* DropSound;

	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ADBItem> ItemClass;
};

UENUM(Blueprintable)
enum class ERarityType : uint8
{
	NONE UMETA(DisplayName = "None"),
	COMMON UMETA(DisplayName = "Common"),
	RARE UMETA(DisplayName = "Rare"),
	EPIC UMETA(DisplayName = "Epic"),
	MAX UMETA(DisplayName = "Max")
};

USTRUCT(Blueprintable)
struct FRarity
{
	GENERATED_BODY()

	void GetRarityType(uint8& Type) const
	{
		Type = (uint8)RarityType;
	}

	uint8 GetRarityType() const
	{
		return (uint8)RarityType;
	}


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERarityType RarityType = ERarityType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

//Defines rarity & rarity value for items, excluding enchantments.
USTRUCT(Blueprintable)
struct FRarityHolder
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FRarity> Rarity;
};

/// <summary>
/// Item data stored in UItemObject, Do not modify anything in SlotHolder during runtime. 
/// Must call Initialize function for FItem to be valid.
/// </summary>
USTRUCT(Blueprintable)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPDA_ItemSlot* ItemSlot;
	UPROPERTY()
	bool bIsValid = false;
	
	UPROPERTY(BlueprintReadOnly)
	FSlotHolder SlotHolder;

	UPROPERTY(BlueprintReadOnly)
	FRarity Rarity;

	UPROPERTY(BlueprintReadOnly)
	FDarkBorneStats Enchantments;

public:
	//Initializers (초기화)
	void Initialize();
	bool IsValid() const;
	const TArray<FRarity>& GetRaritiesFromItemSlot() const;

	//Getters
	const FSlotHolder& GetSlotHolder() const;	
	const FDarkBorneStats& GetEnchantments() const;
	const FRarity& GetRarity() const;
	float GetDefaultValue() const;	
	const ESlotType GetSlotType() const;
			
	//Setters
	void SetRarity(const FRarity& _Rarity);
	void SetEnchantments(const FDarkBorneStats& _Enchantments);
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