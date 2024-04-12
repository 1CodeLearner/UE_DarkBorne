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
enum class ERarity : uint8
{
	NONE UMETA(DisplayName = "None"),
	COMMON UMETA(DisplayName = "Common"),
	RARE UMETA(DisplayName = "Rare"),
	EPIC UMETA(DisplayName = "Epic"),
	MAX UMETA(DisplayName = "Max")
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
	STRENGTH UMETA(DisplayName = "Strength"),
	DEXTERITY UMETA(DisplayName = "Dexterity"),
	KNOWLEDGE UMETA(DisplayName = "Knowledge")
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
struct FPhysicalDamageHolder : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysicalDamages;
};

UENUM()
enum class EEnchantmentType : uint8
{
	ATTRIBUTE UMETA(DisplayName = "Attribute"),
	PHYSICALDAMAGE UMETA(DisplayName = "PhysicalDamage"),
	MAX UMETA(DisplayName = "MAX")
};

USTRUCT(Blueprintable)
struct FEnchantmentsHolder
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysicalDamages;
};

USTRUCT(Blueprintable)
struct FCharacterStat : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FCharacterStat()
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