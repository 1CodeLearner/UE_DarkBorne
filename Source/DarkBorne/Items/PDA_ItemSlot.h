// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ItemTypes/ItemType.h"
#include "PDA_ItemSlot.generated.h"

/**
 *
 */

 class ADBItem;

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
	UPDA_ItemSlot()
	{
		Rarities.Add({ERarityType::COMMON, {0.f, 0.f}});
		Rarities.Add({ERarityType::RARE, {0.f, 0.f}});
		Rarities.Add({ERarityType::EPIC, {0.f, 0.f}});
	};

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	UTexture2D* DisplayImage;

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
