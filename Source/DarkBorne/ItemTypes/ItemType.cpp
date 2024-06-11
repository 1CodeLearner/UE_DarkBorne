// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

bool FItem::IsValid() const
{
	return bIsValid;
}

const TArray<FRarity>& FItem::GetRaritiesFromItemSlot() const
{
	return ItemSlot->Rarities;
}

void FItem::Initialize()
{
	if (ItemSlot)
	{
		SlotHolder.Id = ItemSlot->Id;
		SlotHolder.DisplayMaterial = ItemSlot->DisplayMaterial;
		SlotHolder.IconTexture = ItemSlot->IconTexture;
		SlotHolder.DisplayName = ItemSlot->DisplayName;
		SlotHolder.LoreText = ItemSlot->LoreText;
		SlotHolder.EffectText = ItemSlot->EffectText;
		SlotHolder.InventorySound = ItemSlot->InventorySound;
		SlotHolder.SlotType = ItemSlot->SlotType;
		SlotHolder.SlotDimension = ItemSlot->SlotDimension;
		SlotHolder.EquipSound = ItemSlot->EquipSound;
		SlotHolder.DropSound = ItemSlot->DropSound;
		SlotHolder.ItemType = ItemSlot->ItemType;
		SlotHolder.ItemClass = ItemSlot->ItemClass;

		ItemSlot = nullptr;
		bIsValid = true;
	}
}

const ESlotType FItem::GetSlotType() const
{
	if (!bIsValid)
		return ItemSlot->SlotType;
	else
		return SlotHolder.SlotType;
}

const FRarity& FItem::GetRarity() const
{
	return Rarity;
}

float FItem::GetDefaultValue() const
{
	if (bIsValid)
	{
		return Rarity.Range.min;
	}

	return -1.f;
}

const FSlotHolder& FItem::GetSlotHolder() const
{
	return SlotHolder;
}

const FDarkBorneStats& FItem::GetEnchantments() const
{
	return Enchantments;
}

void FItem::SetRarity(const FRarity& _Rarity)
{
	Rarity = _Rarity;
}

void FItem::SetEnchantments(const FDarkBorneStats& _Enchantments)
{
	Enchantments = _Enchantments;
}
