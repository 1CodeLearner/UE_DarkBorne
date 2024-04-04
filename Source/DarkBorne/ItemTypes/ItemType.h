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
	UPPERWEAR UMETA(DisplayName = "UpperWear"),
	BOTTOMWEAR UMETA(DisplayName = "BottomWear"),
	GLOVES UMETA(DisplayName = "Gloves"),
	BOOTS UMETA(DisplayName = "Boots"),
	CONSUMABLE UMETA(DisplayName = "Consumable")
};

USTRUCT()
struct FLoc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float X;
	UPROPERTY(EditAnywhere)
	float Y;
};
USTRUCT()
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName Id;

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;

	//인벤토리 안에 아이탬 옮기다 소리
	UPROPERTY(EditAnywhere)
	USoundBase* InventorySound;

	UPROPERTY(EditAnywhere)
	ESlotType SlotType;

	//아이템이 인벤토리 차지하는 사이즈
	UPROPERTY(EditAnywhere)
	FLoc SlotDimension;

	//아이템이 현재 있는 coordinates
	UPROPERTY(EditAnywhere)
	FLoc SlotLocation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADBItem> ItemClass;
};

USTRUCT()
struct FWeapon : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADBItem> ItemClass;

	UPROPERTY(EditAnywhere)
	UPDA_ItemSlot* ItemSlot;
};

USTRUCT()
struct FConsumable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADBItem> ItemClass;

	UPROPERTY(EditAnywhere)
	UPDA_ItemSlot* ItemSlot;
};