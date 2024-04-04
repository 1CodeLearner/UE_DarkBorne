// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ItemTypes/ItemType.h"
#include "PDA_ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UPDA_ItemSlot : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere)
	FName Id;

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	USoundBase* EquipSound;

	//인벤토리 안에 아이탬 옮기는 소리
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
