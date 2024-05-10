// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

bool FItem::IsValid() const
{
	return bIsValid;
}

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

		ItemSlot = nullptr;
		bIsValid = true;
	}
}

const TArray<FRarity>& FItem::GetRarities() const
{
	if (!bIsValid)
		return ItemSlot->Rarities;
	else
		return Rarities;
}

const ESlotType FItem::GetSlotType() const
{
	if (!bIsValid)
		return ItemSlot->SlotType;
	else
		return SlotHolder.SlotType;
}

float FItem::GetDefaultValue() const
{
	if (bIsValid && ensureAlways(Rarities.Num() == 1))
	{
		return Rarities[0].Range.min;
	}

	return -1.f;
}