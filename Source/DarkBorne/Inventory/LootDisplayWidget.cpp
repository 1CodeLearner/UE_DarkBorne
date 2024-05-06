// Fill out your copyright notice in the Description page of Project Settings.


#include "LootDisplayWidget.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "../Inventory/InventoryGridWidget.h"
#include "Components/CanvasPanel.h"

void ULootDisplayWidget::StartInit(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp)
{
	if (EquipmentComp) //Dead Player's Inventory
	{
		DisplayPlayerLoot(InventoryComp, EquipmentComp);
	}
	else // non-player inventory (Chests, monsters)
	{
		DisplayOtherLoot(InventoryComp, EquipmentComp);
	}
}

void ULootDisplayWidget::Reset()
{
	if (InventoryLoot_Other)
		InventoryLoot_Other->Reset();
	if (InventoryLoot_Player)
		InventoryLoot_Player->Reset();
	//if(EquipmentGrid_Weapon)
		//EquipmentGrid_Weapon->Reset();
	//if(EquipmentGrid_Consumable)
		//EquipmentGrid_Consumable->Reset();
}

void ULootDisplayWidget::DisplayPlayerLoot(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp)
{
	InventoryLoot_Player->StartInit(InventoryComp);

	InventoryLoot_Player->SetVisibility(ESlateVisibility::Visible);
	InventoryLoot_Other->SetVisibility(ESlateVisibility::Collapsed);

	//Initialize EquipmentGrid_Weapon
	//Initialize EquipmentGrid_Consumable
}

void ULootDisplayWidget::DisplayOtherLoot(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp)
{
	InventoryLoot_Other->StartInit(InventoryComp);

	InventoryLoot_Other->SetVisibility(ESlateVisibility::Visible);
	InventoryLoot_Player->SetVisibility(ESlateVisibility::Collapsed);
}
