// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ItemTypes/ItemType.h"
#include "PDA_ItemSlot.generated.h"

/**
 * 
 */
  
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

USTRUCT(Blueprintable)
struct FDimension
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Y;
};

UCLASS()
class DARKBORNE_API UPDA_ItemSlot : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UTexture2D* DisplayImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FText Description;

	//아이탬 장착할때 내는 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	USoundBase* EquipSound;

	//인벤토리 안에 아이탬 옮기는 소리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	USoundBase* InventorySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	ESlotType SlotType;

	//아이템이 인벤토리 차지하는 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FDimension SlotDimension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<ADBItem> ItemClass;
};
