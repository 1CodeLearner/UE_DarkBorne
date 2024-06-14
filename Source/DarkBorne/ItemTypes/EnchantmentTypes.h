// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnchantmentTypes.generated.h"

USTRUCT(Blueprintable)
struct FRange
{
	GENERATED_USTRUCT_BODY()

	FRange operator+(const FRange& other) const {
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

	//Any elements beyond _EnchantmentMark_ cannot have enchantments. 
	//_Enchantment Mark_ 후의 enum 는 추가 스탯을 가질 수 없습니다.
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

	FAttribute operator+(const FAttribute& other) const {
		ensureAlways(AttributeType == other.AttributeType);
		return FAttribute({ AttributeType, Range + other.Range });
	}
	FAttribute operator-(const FAttribute& other) {
		ensureAlways(AttributeType == other.AttributeType);
		return FAttribute({ AttributeType, Range - other.Range });
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
	PHYSICALDAMAGEBONUS UMETA(DisplayName = "Physical Damage Bonus"),
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

UENUM()
enum class EDarkBornStatType : uint8
{
	ATTRIBUTE UMETA(DisplayName = "Attribute"),
	PHYSICALDAMAGE UMETA(DisplayName = "Physical Damage"),
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


/// <summary>
/// Stats used in damage calculations;
/// </summary>
USTRUCT(Blueprintable)
struct FFinalStat
{
	GENERATED_BODY();

	FFinalStat()
	{
		WeaponDamage = 0.f;
		DamageBlockAmt = 0.f;
	}

	FFinalStat(float WeaponDmg, TArray<FAttribute> Attris, TArray<FPhysicalDamage> PhysDmgs, float DamageBlockAmount)
	{
		WeaponDamage = WeaponDmg; 
		Attributes = std::move(Attris);
		PhysDamages = std::move(PhysDmgs);
		DamageBlockAmt = DamageBlockAmount;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysDamages;

	UPROPERTY(VisibleAnywhere)
	float DamageBlockAmt;
};

/// <summary>
/// Stats accumulated with equipped items in UDBEquipmentComponent. Elements of each Array are sorted in enum types
/// </summary>
USTRUCT(Blueprintable)
struct FAddedStat
{
	GENERATED_USTRUCT_BODY()

	float health = 0.f;

	FAddedStat()
	{
		health = 0.f;

		Attributes.Add({ EAttributeType::STRENGTH, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::DEXTERITY, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::KNOWLEDGE, {0.f, 0.f} });

		PhysDamages.Add({ EPhysicalDamageType::PHYSICALDAMAGEBONUS, {0.f, 0.f} });
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WeaponDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPhysicalDamage> PhysDamages;

	UPROPERTY(VisibleAnywhere)
	float DamageBlockAmt;
};


/// <summary>
/// Base stats setup in DataTable using FCharacterBaseStat are assigned to this.
/// </summary>
USTRUCT(BlueprintType)
struct FBaseStat
{
	GENERATED_USTRUCT_BODY()

	FBaseStat()
	{
		Attributes.Add({ EAttributeType::STRENGTH, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::DEXTERITY, {0.f, 0.f} });
		Attributes.Add({ EAttributeType::KNOWLEDGE, {0.f, 0.f} });
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FAttribute> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float health = 0.f;
};

/// <summary>
/// Character's stats without any equipment
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