// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

void FItem::Initialize()
{
	if (ItemSlot)
	{
		SlotHolder.Id = ItemSlot->Id;
		SlotHolder.DisplayMaterial = ItemSlot->DisplayMaterial;
		SlotHolder.DisplayName = ItemSlot->DisplayName;
		SlotHolder.LoreText = ItemSlot->LoreText;
		SlotHolder.InventorySound = ItemSlot->InventorySound;
		SlotHolder.SlotType = ItemSlot->SlotType;
		SlotHolder.SlotDimension = ItemSlot->SlotDimension;
		SlotHolder.EquipSound = ItemSlot->EquipSound;
		SlotHolder.ItemType = ItemSlot->ItemType;
		SlotHolder.ItemClass = ItemSlot->ItemClass;
	}
	ItemSlot = nullptr;
}
