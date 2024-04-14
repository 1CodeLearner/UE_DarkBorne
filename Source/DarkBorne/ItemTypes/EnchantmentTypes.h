// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnchantmentTypes.generated.h"

USTRUCT(Blueprintable)
struct FRange
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float min;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float max;
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
	ERarityType RarityType;

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
	KNOWLEDGE UMETA(DisplayName = "Knowledge")
};


/// <summary>
/// basic stats, such as Strength, Dexterity, and Will
/// </summary>
USTRUCT(Blueprintable)
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttributeType AttributeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(BlueprintType)
struct FAttributeStat
{
	GENERATED_USTRUCT_BODY()

	float Strength;
	float Dexterity; 
	float Knowledge;
};

USTRUCT(Blueprintable)
struct FAttributeHolder : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;
};

UENUM(BlueprintType)
enum class EPhysicalDamageType : uint8
{
	PHYSICALDAMAGEBONUS UMETA(DisplayName = "PhysicalDamageBonus")
};

/// <summary>
/// Additional physical damage that weapons deal.
/// </summary>
USTRUCT(Blueprintable)
struct FPhysicalDamage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPhysicalDamageType PhysicalDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRange Range;
};

USTRUCT(Blueprintable)
struct FPhysicalDamageStat
{
	GENERATED_USTRUCT_BODY()

	float PhysicalDamageBonus;
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
	float health;
};

/// <summary>
/// Stats that are randomly chosen for high rarity equipments.
/// </summary>
UENUM()
enum class EDarkBornStatType : uint8
{
	ATTRIBUTE UMETA(DisplayName = "Attribute"),
	PHYSICALDAMAGE UMETA(DisplayName = "PhysicalDamage"),
	MAX UMETA(DisplayName = "MAX")
};


/// <summary>
/// Stats pulled from Data Table. These stats are not sorted/processed.
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

/// <summary>
/// Stats used for damage calculation and UI display 
/// </summary>
USTRUCT(Blueprintable)
struct FFinalStat
{
	GENERATED_USTRUCT_BODY()

	FAttributeStat AttributeStat;

	FPhysicalDamageStat PhysDmgStat;
};