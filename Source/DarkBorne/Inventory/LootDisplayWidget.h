// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootDisplayWidget.generated.h"

/**
 *
 */

class UPlayerEquipmentComponent;
class UDBEquipmentComponent;

class UInventoryGridWidget;

class UCanvasPanel;

UCLASS()
class DARKBORNE_API ULootDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void StartInit(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp);
	void Reset();

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Player;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Other;

	void DisplayPlayerLoot(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp);
	void DisplayOtherLoot(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp);
	//EquipmentGrid_Weapon
	//EquipmentGrid_Consumable

};
