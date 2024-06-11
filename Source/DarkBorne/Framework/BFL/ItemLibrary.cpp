// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLibrary.h"
#include "Engine/World.h"
#include "../../Items/DBItem.h"
#include "../../Inventory/ItemObject.h"

int32 UItemLibrary::GetSlotIndexByObject(UItemObject* ItemObj)
{
	ESlotType Slot = ItemObj->GetItemData().GetSlotHolder().SlotType;
	return GetSlotIndexByEnum(Slot);
}

int32 UItemLibrary::GetSlotIndexByEnum(ESlotType SlotType)
{
	if (SlotType < ESlotType::_ENCHANTMENTMARK_)
	{
		return int32(SlotType);
	}
	else
	{
		return int32(SlotType) - 1;
	}
}
