// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseGridWidget.generated.h"

/**
 *
 */


//UENUM(BlueprintType)
//enum class EGridWidgetType : uint8
//{
//	PLAYERINVENTORY UMETA(DisplayName = "PlayerInventory"),
//	PLAYEREQUIPMENT UMETA(DisplayName = "PlayerEquipment"),
//	LOOTINVENTORY UMETA(DisplayName = "LootInventory"),
//	LOOTEQUIPMENT UMETA(DisplayName = "LootEquipment")
//};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemClickedDelegate, UBaseItemWidget*, ItemWidgetClicked, EGridWidgetType, GridWidgetType, bool, bIsRightClick);

UCLASS()
class DARKBORNE_API UBaseGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/*UPROPERTY(BlueprintCallable)
	FItemClickedDelegate OnItemClicked;*/
protected:
	/*UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
	EGridWidgetType GridWidgetType;*/
};
