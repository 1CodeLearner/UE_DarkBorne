// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

int32 FItem::GetSlotIndex() const
{
	ESlotType Slot = ItemSlot->SlotType;

	if (Slot < ESlotType::_ENCHANTMENTMARK_) {
		return int32(Slot);
	}
	else {
		return int32(Slot) - 1;
	}
}
