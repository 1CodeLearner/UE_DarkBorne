// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../Items/DBItem.h"
#include "Kismet/GameplayStatics.h"

UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bIsDirty = false;
	Columns = 8;
	Rows = 5;
	TileSize = 50.f;
}

bool UPlayerEquipmentComponent::HasRoomFor(UItemObject* ItemObject) const
{
	if (!IsValid(ItemObject)) return false;

	for (int i = 0; i < Items.Num(); i++)
	{
		//		UE_LOG(LogTemp, Warning, TEXT("TaxiToServer and this are diff"));
		if (IsRoomAvailable(ItemObject, i))
		{
			return true;
		}
		else continue;
	}

	return false;
}

bool UPlayerEquipmentComponent::HasItem(UItemObject* ItemObject) const
{
	FIntPoint dim = ItemObject->GetDimentions();
	int32 total = dim.X * dim.Y;

	int32 count = 0;
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i] == ItemObject)
		{
			count++;
		}
	}

	return total == count;
}

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	Items.SetNum(Columns * Rows);
}

bool UPlayerEquipmentComponent::TryAddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	if (!IsValid(ItemObject)) return false;
	if (!TaxiToServer) return false;
	//if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
	//	this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
	//	return false;

	for (int i = 0; i < Items.Num(); i++)
	{
		//		UE_LOG(LogTemp, Warning, TEXT("TaxiToServer and this are diff"));
		if (IsRoomAvailable(ItemObject, i))
		{
			if (this->GetOwner()->HasNetOwner())
			{
				UE_LOG(LogTemp, Warning, TEXT("Has LocalNetOwner()"));
				Server_AddItemAt(ItemObject, i);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Has no LocalNetOwner()"));
				auto PEComp = Cast<UPlayerEquipmentComponent>(TaxiToServer);
				PEComp->Server_TaxiForAddItemAt(ItemObject, i, this);
			}

			return true;
		}
		else continue;
	}
	return false;
}

void UPlayerEquipmentComponent::RemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	if (!TaxiToServer) return;
	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/
	
	if (HasItem(ItemObject)) 
	{
		if (this->GetOwner()->HasNetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
			Server_RemoveItem(ItemObject);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
			auto PEComp = Cast<UPlayerEquipmentComponent>(TaxiToServer);
			PEComp->Server_TaxiForRemoveItem(ItemObject, this);
		}
	}
}

void UPlayerEquipmentComponent::AddItemAt(UItemObject* ItemObject, int32 index, UBaseInventoryComponent* TaxiToServer)
{
	if (!IsValid(ItemObject)) return;
	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/

	if (this->GetOwner()->HasNetOwner() && this->GetOwner()->GetIsReplicated())
	{
		UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
		Server_AddItemAt(ItemObject, index);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
		auto PEComp = Cast<UPlayerEquipmentComponent>(TaxiToServer);
		PEComp->Server_TaxiForAddItemAt(ItemObject, index, this);
	}
}

void UPlayerEquipmentComponent::Server_TaxiForAddItemAt_Implementation(UItemObject* ItemObject, int32 TopLeftIndex,
	UBaseInventoryComponent* TaxiedInventoryComp)
{
	auto TaxiedPEComp = Cast<UPlayerEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedPEComp))
	{
		TaxiedPEComp->Server_AddItemAt(ItemObject, TopLeftIndex);
	}
}

void UPlayerEquipmentComponent::Server_TaxiForRemoveItem_Implementation(UItemObject* ItemObject, UBaseInventoryComponent* TaxiedInventoryComp)
{
	auto TaxiedPEComp = Cast<UPlayerEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedPEComp))
	{
		TaxiedPEComp->Server_RemoveItem(ItemObject);
	}
}

void UPlayerEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject)) return;
	if (HasItem(ItemObject)) 
	{
		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (Items[i] == ItemObject)
			{
				Items[i] = nullptr;
			}
		}
		OnRep_Items();
	}	
}

int32 UPlayerEquipmentComponent::GetColumn() const
{
	return Columns;
}

int32 UPlayerEquipmentComponent::GetRow() const
{
	return Rows;
}

void UPlayerEquipmentComponent::Server_AddItemAt_Implementation(UItemObject* ItemObject, int32 TopLeftIndex)
{
	////ForEachIndex
	if (!ItemObject)
		return;

	//If inventory already has ItemObject, remove it from inventory.
	if (HasItem(ItemObject)) 
	{
		RemoveItem(ItemObject, this);
	}
	
	if (IsRoomAvailable(ItemObject, TopLeftIndex)) 
	{
		FTile refTile = IndexToTile(TopLeftIndex);
		FIntPoint dimentions = ItemObject->GetDimentions();
		FTile newTile;

		for (int32 i = refTile.X; i < refTile.X + (dimentions.X); i++)
		{
			for (int32 j = refTile.Y; j < refTile.Y + (dimentions.Y); j++)
			{
				newTile.X = i;
				newTile.Y = j;
				Items[TileToIndex(newTile)] = ItemObject;
			}
		}

		ItemObject->TryDestroyItemActor();
		OnRep_Items();
	}
	else 
	{
		//Re-add it back to inventory.
		TryAddItem(ItemObject, this);
	}


}

TMap<class UItemObject*, FTile> UPlayerEquipmentComponent::GetAllItems() const
{
	TMap<UItemObject*, FTile> AllItems;
	UItemObject* CurrentItemObject;

	for (int i = 0; i < Items.Num(); i++)
	{
		CurrentItemObject = Items[i];
		if (IsValid(CurrentItemObject) && !AllItems.Contains(CurrentItemObject))
		{
			AllItems.Add(CurrentItemObject, IndexToTile(i));
		}
	}
	return AllItems;
}

FTile UPlayerEquipmentComponent::IndexToTile(int32 Index) const
{
	FTile Result;
	Result.X = Index % Columns;
	Result.Y = Index / Columns;
	return Result;
}

int32 UPlayerEquipmentComponent::TileToIndex(FTile Tile) const
{
	int32 value = Tile.X + Tile.Y * Columns;
	return value;
}

bool UPlayerEquipmentComponent::IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex) const
{
	//ForEachIndex
	FTile refTile = IndexToTile(TopLeftIndex);
	FIntPoint dimentions = ItemObject->GetDimentions();
	FTile newTile;
	for (int32 i = refTile.X; i < refTile.X + (dimentions.X); i++)
	{
		for (int32 j = refTile.Y; j < refTile.Y + (dimentions.Y); j++)
		{
			newTile.X = i;
			newTile.Y = j;

			//isTileValid
			if (IsTileValid(newTile))
			{
				int32 num = TileToIndex(newTile);
				//if(Items.IsValidIndex(num))
				//	return false;

				TTuple<bool, UItemObject*> output = GetItematIndex(num);
				bool bHasItem = output.Get<0>();
				UItemObject* ExistingItemObject = output.Get<1>();
				if (bHasItem)
				{
					if (IsValid(ExistingItemObject))
					{
						return false;
					}
				}
			}
			else {
				return false;
			}

		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Valid")));
	return true;
}

inline TTuple<bool, UItemObject*> UPlayerEquipmentComponent::GetItematIndex(int32 Index) const
{
	if (ensureAlways(Items.IsValidIndex(Index)))
		return MakeTuple(IsValid(Items[Index]), Items[Index]);
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("index out of bounds: %d"), Index);
		return MakeTuple(false, nullptr);
	}
}

bool UPlayerEquipmentComponent::IsTileValid(FTile tile) const
{
	if (tile.X >= 0 && tile.Y >= 0 && tile.X < Columns && tile.Y < Rows)
	{
		return true;
	}

	return false;

}
