// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "../Items/PDA_ItemSlot.h"
#include "../Inventory/ItemObject.h"


// Sets default values for this component's properties
UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




void UPlayerEquipmentComponent::BeginPlay()
{
	
	itemArray.SetNum(Columns * Rows);
}

void UPlayerEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

bool UPlayerEquipmentComponent::TryAddItem(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject)) return false;
	for(int i = 0; i <= itemArray.Num(); i++)
	{
		if (IsRoomAvailable(ItemObject, i))
		{
			AddItemAt(ItemObject,i);
			return true;
		}
		else continue;
	}
	return false;
}

TMap<class UItemObject*, FTile> UPlayerEquipmentComponent::GetAllItems()
{
	TMap<UItemObject*, FTile> AllItems;
	UItemObject* CurrentItemObject;

	for (int i = 0; i < itemArray.Num(); i++)
	{
		CurrentItemObject = itemArray[i];
		if (IsValid(CurrentItemObject) && !AllItems.Contains(CurrentItemObject))
		{
			AllItems.Add(CurrentItemObject, IndexToTile(i));
		}
	}
	return AllItems;
}

inline FTile UPlayerEquipmentComponent::IndexToTile(int32 Index)
{
	FTile Result;
	Result.X = Index % Columns;
	Result.Y = Index / Columns;
	return Result;
}

int32 UPlayerEquipmentComponent::TileToIndex(FTile Tile)
{
	int32 value = Tile.X + Tile.Y * Columns;
	return value;
}

void UPlayerEquipmentComponent::AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex)
{
	////ForEachIndex
	FTile refTile = IndexToTile(TopLeftIndex);
	FIntPoint dimentions = ItemObject->GetDimentions();
	FTile newTile;
	for (int32 i = refTile.X; i < refTile.X + (dimentions.X - 1); i++)
	{
		for (int32 j = refTile.Y; j < refTile.Y + (dimentions.Y - 1); i++)
		{
			newTile.X = i;
			newTile.Y = j;
			itemArray[TileToIndex(newTile)] = ItemObject;
		}
	}
	isDirty = true;

}

bool UPlayerEquipmentComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex)
{
	//ForEachIndex
	FTile refTile = IndexToTile(TopLeftIndex);
	FIntPoint dimentions = ItemObject->GetDimentions();
	FTile newTile;
	for (int32 i = refTile.X; i < refTile.X + (dimentions.X - 1); i++)
	{
		for (int32 j = refTile.Y; j < refTile.Y + (dimentions.Y - 1); i++)
		{
			newTile.X = i;
			newTile.Y = j;
			
			//isTileValid
			if (IsTileValid(newTile))
			{
				int32 num = TileToIndex(newTile);
				TTuple<bool,UItemObject*> output = GetItematIndex(num);
				bool valid = output.Get<0>();
				UItemObject* outItemObject= output.Get<1>();
				if (valid)
				{
					if (IsValid(outItemObject))
					{
						return false;
					}
				}
				else return false;
			}
			else return false;

		}
	}
	return true;
	
}

void UPlayerEquipmentComponent::RemoveItem(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject)) return;
	for (int32 i = 0; i < itemArray.Num(); i++)
	{
		if (itemArray[i] == ItemObject)
		{
			itemArray[i] = nullptr; 
			isDirty = true; 
		}
	}
}

inline TTuple<bool,UItemObject*> UPlayerEquipmentComponent::GetItematIndex(int32 Index)
{
	TTuple<bool, UItemObject*> returnTuple;
	if (IsValid(itemArray[Index]))
	{
		return MakeTuple(true,itemArray[Index]);
	}
	return MakeTuple(false,nullptr);
}

inline bool UPlayerEquipmentComponent::IsTileValid(FTile tile)
{
	if (tile.X >= 0 && tile.Y >= 0 && tile.X < Columns && tile.Y < Columns)
	{
		return true;
	}
	
		return false;
	
}
