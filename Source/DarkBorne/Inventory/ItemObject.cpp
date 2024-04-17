// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "../Items/PDA_ItemSlot.h"
#include "Materials/MaterialInterface.h"
#include "../Items/DBItem.h"

void UItemObject::Initialize(FItem item)
{
	this->Item = Item;
}

FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	//return _dimentions;
	FIntPoint Temp(Item.ItemSlot->SlotDimension.X, Item.ItemSlot->SlotDimension.Y);
	return Temp;
}

UMaterialInterface* UItemObject::GetIcon()
{
	return Item.ItemSlot->DisplayMaterial;
}

TSubclassOf<AActor> UItemObject::GetItemClass()
{
	return Item.ItemSlot->ItemClass;
}

UWorld* UItemObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}