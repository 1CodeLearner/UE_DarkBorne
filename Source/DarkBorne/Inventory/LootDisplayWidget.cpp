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
		OtherInventoryComp = InventoryComp;
		OtherEquipmentComp = EquipmentComp;
		
		InventoryLoot_Player->StartInit(OtherInventoryComp);
		WidgetSwitcher_Loot->SetActiveWidgetIndex(0);
	}
	else // non-player inventory (Chests, monsters)
	{
		OtherInventoryComp = InventoryComp;
		OtherEquipmentComp = nullptr;

		InventoryLoot_Other->StartInit(OtherInventoryComp);
		WidgetSwitcher_Loot->SetActiveWidgetIndex(1);
	}
}

void ULootDisplayWidget::Reset()
{
	//Clear Inventory Component references in WBP_LootDisplay	
	OtherInventoryComp = nullptr;
	OtherEquipmentComp = nullptr;

	InventoryLoot_Player->Refresh();
	InventoryLoot_Other->Refresh();
	//Clear Item Images in WBP_LootDisplay
}
