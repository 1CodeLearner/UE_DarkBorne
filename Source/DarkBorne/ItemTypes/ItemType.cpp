// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemType.h"
#include "../Items/PDA_ItemSlot.h"

FSlotHolder::FSlotHolder(UPDA_ItemSlot* DataAsset)
{
	if (ensureAlways(DataAsset))
	{
		DisplayMaterial = DataAsset->DisplayMaterial;
		DisplayName = DataAsset->DisplayName;
		SlotType = DataAsset->SlotType;
		ItemClass = DataAsset->ItemClass;
	}

}
