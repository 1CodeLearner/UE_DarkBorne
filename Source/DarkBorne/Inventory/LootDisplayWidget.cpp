// Fill out your copyright notice in the Description page of Project Settings.


#include "LootDisplayWidget.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "../Inventory/InventoryGridWidget.h"
#include "Components/WidgetSwitcher.h"

void ULootDisplayWidget::StartInit(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp)
{
	if (EquipmentComp) //Dead Player's Inventory
	{
		InventoryLoot_Player->StartInit(InventoryComp);
		//Initialize EquipmentGrid_Weapon
		//Initialize EquipmentGrid_Consumable
		WidgetSwitcher_Loot->SetActiveWidgetIndex(0);
	}
	else // non-player inventory (Chests, monsters)
	{
		InventoryLoot_Other->StartInit(InventoryComp);
		WidgetSwitcher_Loot->SetActiveWidgetIndex(1);
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
