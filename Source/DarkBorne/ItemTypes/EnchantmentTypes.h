// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnchantmentTypes.generated.h"

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
struct FRange
{
	GENERATED_USTRUCT_BODY()

	FRange operator+(const FRange& other) {
		return FRange(min + other.min, max + other.max);
	}
	FRange operator-(const FRange& other) {
		ensureAlways(min - other.min >= 0.f && max - other.max >= 0.f);
		return FRange(min - other.min, max - other.max);
	}
	FRange& operator+=(const FRange& other) {
		min += other.min;
		max += other.max;
		return *this;
	}
	FRange& operator-=(const FRange& other) {
		ensureAlways(min - other.min >= 0.f && max - other.max >= 0.f);
		min -= other.min;
		max -= other.max;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float min = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float max = 0.f;
};


UENUM(Blueprintable)
enum class ESlotType : uint8
{
	WEAPON = 0 UMETA(DisplayName = "Weapon"),
	HEAD UMETA(DisplayName = "Head"),
	UPPERWEAR UMETA(DisplayName = "UpperWear"),
	BOTTOMWEAR UMETA(DisplayName = "BottomWear"),
	GLOVES UMETA(DisplayName = "Gloves"),
	BOOTS UMETA(DisplayName = "Boots"),

	//Any elements beyond _EnchantmentMark_ cannot have enchantments 
	_ENCHANTMENTMARK_ UMETA(DisplayName = "_EnchantmentMark_"),

	UTILITY UMETA(DisplayName = "Utility"),
	CONSUMABLE UMETA(DisplayName = "Consumable"),
	NONE UMETA(DisplayName = "None"),
	MAX UMETA(DisplayName = "Max")
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
	GENERATED_USTRUCT_BODY()

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
/// Each element represents an index of Attribute arrays in FCharacterStat. DO NOT change the orders of the elements
/// </summary>
UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	STRENGTH UMETA(DisplayName = "Strength"),
	DEXTERITY UMETA(DisplayName = "Dexterity"),
	KNOWLEDGE UMETA(DisplayName = "Knowledge"),
	MAX UMETA(DisplayName = "Max")
};


/// <summary>
/// basic stats, such as Strength, Dexterity, and Will
/// </summary>
USTRUCT(Blueprintable)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	FAttribute operator+(const FAttribute& other) {
		ensureAlways(AttributeType == other.AttributeType);
		return FAttribute({ AttributeType, Range + other.Range });
	}
	FAttribute operator-(const FAttribute& other) {
		ensureAlways(AttributeType == other.AttributeType);
		return FAttribute({AttributeType, Range - other.Range});
	}
	FAttribute& operator+=(const FAttribute& other) {
		ensureAlways(AttributeType == other.AttributeType);
		Range += other.Range;
		return *this;
	}
	FAttribute& operator-=(const FAttribute& other) {
		ensureAlways(AttributeType == other.AttributeType);
		Range -= other.Range;
		return *this;
	}


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttributeType AttributeType = EAttributeType::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(Blueprintable)
struct FAttributeHolder : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;
};

/// <summary>
/// DO NOT change the orders of the elements
/// </summary>
UENUM(BlueprintType)
enum class EPhysicalDamageType : uint8
{
	PHYSICALDAMAGEBONUS UMETA(DisplayName = "PhysicalDamageBonus"),
	MAX UMETA(DisplayName = "Max")
};

/// <summary>
/// Additional physical damage that weapons deal.
/// </summary>
USTRUCT(Blueprintable)
struct FPhysicalDamage
{
	GENERATED_USTRUCT_BODY()

	FPhysicalDamage operator+(const FPhysicalDamage& other) {
		ensureAlways(PhysicalDamageType == other.PhysicalDamageType);
		return FPhysicalDamage({ PhysicalDamageType, Range + other.Range });
	}
	FPhysicalDamage operator-(const FPhysicalDamage& other) {
		ensureAlways(PhysicalDamageType == other.PhysicalDamageType);
		return FPhysicalDamage({ PhysicalDamageType, Range - other.Range });
	}
	FPhysicalDamage& operator+=(const FPhysicalDamage& other) {
		ensureAlways(PhysicalDamageType == other.PhysicalDamageType);
		Range += other.Range;
		return *this;
	}
	FPhysicalDamage& operator-=(const FPhysicalDamage& other) {
		ensureAlways(PhysicalDamageType == other.PhysicalDamageType);
		Range -= other.Range;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPhysicalDamageType PhysicalDamageType = EPhysicalDamageType::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(Blueprintable)
struct FPhysicalDamageHolder : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysicalDamages;
};

/// <summary>
/// Character's stats without any equipments
/// </summary>
USTRUCT(Blueprintable)
struct FCharacterBaseStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterBaseStat()
	{
		Attributes.Add({ EAttributeType::STRENGTH, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::DEXTERITY, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::KNOWLEDGE, {0.f, 0.f} });
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float health = 0.f;
};

UENUM()
enum class EDarkBornStatType : uint8
{
	ATTRIBUTE UMETA(DisplayName = "Attribute"),
	PHYSICALDAMAGE UMETA(DisplayName = "PhysicalDamage"),
	MAX UMETA(DisplayName = "MAX")
};


/// <summary>
/// Stats pulled from Data Table and assigned to items. These stats are not sorted. They are later added to FFinalStat for damage calculations
/// </summary>
USTRUCT(Blueprintable)
struct FDarkBorneStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysicalDamages;
};

//USTRUCT(BlueprintType)
//struct FAttributeStat
//{
//	GENERATED_USTRUCT_BODY()
//
//	float Strength;
//	float Dexterity;
//	float Knowledge;
//};
//
//USTRUCT(Blueprintable)
//struct FPhysicalDamageStat
//{
//	GENERATED_USTRUCT_BODY()
//
//	float PhysicalDamageBonus;
//};

/// <summary>
/// Stats used for damage calculation and UI display. Values are sorted by their enum types
/// </summary>
USTRUCT(Blueprintable)
struct FFinalStat
{
	GENERATED_USTRUCT_BODY()

	float health = 0.f;

	FFinalStat()
	{
		health = 0.f;

		Attributes.Add({ EAttributeType::STRENGTH, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::DEXTERITY, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::KNOWLEDGE, {0.f, 0.f} });

		PhysDamages.Add({ EPhysicalDamageType::PHYSICALDAMAGEBONUS, {0.f, 0.f} });
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysDamages;
};