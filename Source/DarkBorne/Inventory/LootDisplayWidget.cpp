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
	InventoryLoot_Other->Reset();
	InventoryLoot_Player->Reset();
}
