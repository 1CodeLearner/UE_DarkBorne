// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

void FItem::Initialize()
{
	if (ItemSlot)
	{
		SlotHolder.DisplayMaterial = ItemSlot->DisplayMaterial;
		SlotHolder.DisplayName = ItemSlot->DisplayName;
		SlotHolder.SlotType = ItemSlot->SlotType;
		SlotHolder.ItemClass = ItemSlot->ItemClass;
		ItemSlot = nullptr;
	}
}
