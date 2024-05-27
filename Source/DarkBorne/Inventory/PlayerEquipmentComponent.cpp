// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../Items/DBItem.h"
#include "Kismet/GameplayStatics.h"
#include "../DBCharacters/DBCharacter.h"
#include "DBEquipmentComponent.h"

UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bIsDirty = false;
}

bool UPlayerEquipmentComponent::HasRoomFor(UItemObject* ItemObject) const
{
	if (!IsValid(ItemObject)) return false;

	for (int i = 0; i < InventoryItems.Items.Num(); i++)
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
	for (int32 i = 0; i < InventoryItems.Items.Num(); i++)
	{
		if (InventoryItems.Items[i] == ItemObject)
		{
			count++;
		}
	}

	return total == count;
}

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryItems.Items.SetNum(Columns * Rows);
}

bool UPlayerEquipmentComponent::TryAddItem(UItemObject* ItemObject, AActor* InitiatedActor)
{
	if (!IsValid(ItemObject)) return false;
	if (!InitiatedActor) return false;
	//if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
	//	this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
	//	return false;

	for (int i = 0; i < InventoryItems.Items.Num(); i++)
	{
		//		UE_LOG(LogTemp, Warning, TEXT("TaxiToServer and this are diff"));
		if (IsRoomAvailable(ItemObject, i))
		{
			if (GetOwner()->HasNetOwner())
			{
				UE_LOG(LogTemp, Warning, TEXT("Has LocalNetOwner()"));
				Server_AddItemAt(ItemObject, i, InitiatedActor);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Has no LocalNetOwner()"));
				auto PEComp = InitiatedActor->GetComponentByClass<UPlayerEquipmentComponent>();
				PEComp->Server_TaxiForAddItemAt(this, ItemObject, i, InitiatedActor);
			}

			return true;
		}
		else continue;
	}
	return false;
}

void UPlayerEquipmentComponent::AddItemAt(UItemObject* ItemObject, int32 index, AActor* InitiatedActor)
{
	if (!IsValid(ItemObject)) return;
	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/

	if (GetOwner()->HasNetOwner() && GetOwner()->GetIsReplicated())
	{
		UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
		Server_AddItemAt(ItemObject, index, InitiatedActor);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
		auto PEComp = InitiatedActor->GetComponentByClass<UPlayerEquipmentComponent>();
		PEComp->Server_TaxiForAddItemAt(this, ItemObject, index, InitiatedActor);
	}
}

void UPlayerEquipmentComponent::Server_TaxiForAddItemAt_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, int32 TopLeftIndex, AActor* InitiatedActor)
{
	auto TaxiedPEComp = Cast<UPlayerEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedPEComp))
	{
		TaxiedPEComp->Server_AddItemAt(ItemObject, TopLeftIndex, InitiatedActor);
	}
}

void UPlayerEquipmentComponent::Server_AddItemAt_Implementation(UItemObject* ItemObject, int32 TopLeftIndex, AActor* InitiatedActor)
{
	////ForEachIndex
	if (!ItemObject)
		return;

	FInventoryItems Old = InventoryItems;

	//If inventory already has ItemObject, remove it from inventory.
	if (HasItem(ItemObject))
	{
		RemoveItem(ItemObject, InitiatedActor);
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
				InventoryItems.Items[TileToIndex(newTile)] = ItemObject;
			}
		}

		ItemObject->TryDestroyItemActor();

		InventoryItems.InteractingActor = InitiatedActor;

		OnRep_Items(Old);
	}
	else
	{
		//Re-add it back to inventory.
		TryAddItem(ItemObject, InitiatedActor);
	}


}

void UPlayerEquipmentComponent::RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor)
{
	if (!InitiatedActor) return;
	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/

	if (HasItem(ItemObject))
	{
		if (GetOwner()->HasNetOwner())
		{
			UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
			Server_RemoveItem(ItemObject, InitiatedActor);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
			auto PEComp = InitiatedActor->GetComponentByClass<UPlayerEquipmentComponent>();
			PEComp->Server_TaxiForRemoveItem(this, ItemObject, InitiatedActor);
		}
	}
}

void UPlayerEquipmentComponent::Server_TaxiForRemoveItem_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor)
{
	auto TaxiedPEComp = Cast<UPlayerEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedPEComp))
	{
		TaxiedPEComp->Server_RemoveItem(ItemObject, InitiatedActor);
	}
}

void UPlayerEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject, AActor* InitiatedActor)
{
	if (!IsValid(ItemObject)) return;

	FInventoryItems Old = InventoryItems;

	if (HasItem(ItemObject))
	{
		for (int32 i = 0; i < InventoryItems.Items.Num(); i++)
		{
			if (InventoryItems.Items[i] == ItemObject)
			{
				InventoryItems.Items[i] = nullptr;
			}
		}
		InventoryItems.InteractingActor = InitiatedActor;

		OnRep_Items(Old);
	}
}

void UPlayerEquipmentComponent::ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	if (!ItemObject)
		return;
	if (!(InitiatedActor && InitiatedActor->HasNetOwner()))
		return;
	if (!HasItem(ItemObject))
		return;

	if (GetOwner()->HasNetOwner())
	{
		Server_ProcessPressInput(ItemObject, InitiatedActor, InventoryInput);
	}
	else
	{
		auto TaxiToServer = InitiatedActor->GetComponentByClass<UPlayerEquipmentComponent>();
		TaxiToServer->Server_TaxiForProcessPressInput(this, ItemObject, InitiatedActor, InventoryInput);
	}

}

void UPlayerEquipmentComponent::Server_TaxiForProcessPressInput_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	auto Taxied = Cast<UPlayerEquipmentComponent>(TaxiedInventoryComp);
	if (Taxied && ensureAlways(Taxied != this))
	{
		UE_LOG(LogTemp, Warning, TEXT("Taxied"));
		Taxied->Server_ProcessPressInput(ItemObject, InitiatedActor, InventoryInput);
	}
}

void UPlayerEquipmentComponent::Server_ProcessPressInput_Implementation(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory ProcessPressInput"));

	if (!ItemObject)
		return;
	if (!InitiatedActor)
		return;
	if (!HasItem(ItemObject))
		return;

	UPlayerEquipmentComponent* PlayerInventory = InitiatedActor->GetComponentByClass<UPlayerEquipmentComponent>();
	if (!PlayerInventory)
		return;

	UPlayerEquipmentComponent* From = this;

	if (InventoryInput.bHasRightClicked)
	{
		if (InventoryInput.bHasShiftClicked)
		{
			From->RemoveItem(ItemObject, InitiatedActor);
			Server_SpawnItem(InitiatedActor, ItemObject, false);
		}
		else
		{
			UDBEquipmentComponent* To;
			To = InitiatedActor->GetComponentByClass<UDBEquipmentComponent>();
			if (To)
			{
				UItemObject* EquippedItem = nullptr;
				if (!To->IsSlotVacant(ItemObject)) //If Player is already equipping an item
				{
					EquippedItem = To->GetSlotItem(ItemObject->GetSlotType());
				}

				From->RemoveItem(ItemObject, InitiatedActor);

				if (EquippedItem)
				{

					if (PlayerInventory->TryAddItem(EquippedItem, InitiatedActor)) //if adding equipped item to player's inventory was successful
					{
						To->RemoveItem(EquippedItem, InitiatedActor);
						To->AddItem(ItemObject, InitiatedActor); //Equip ItemObject 
					}
					else
					{
						From->TryAddItem(ItemObject, InitiatedActor); //Put ItemObject back
					}
				}
				else
				{
					To->AddItem(ItemObject, InitiatedActor); //Equip Item
				}
			}
		}
	}
	else
	{
		UPlayerEquipmentComponent* To;
		To = PlayerInventory;

		if (To)
		{
			From->RemoveItem(ItemObject, InitiatedActor);

			if (!To->TryAddItem(ItemObject, InitiatedActor)) // if adding ItemObject was unsuccessful
			{
				From->TryAddItem(ItemObject, InitiatedActor); // Put ItemObject back
			}
		}
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

TMap<class UItemObject*, FTile> UPlayerEquipmentComponent::GetAllItems() const
{
	TMap<UItemObject*, FTile> AllItems;
	UItemObject* CurrentItemObject;

	for (int i = 0; i < InventoryItems.Items.Num(); i++)
	{
		CurrentItemObject = InventoryItems.Items[i];
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

void UPlayerEquipmentComponent::OnRep_Items(FInventoryItems Old)
{
	Super::OnRep_Items(Old);

	TArray<UItemObject*> ItemUpdated;
	for (int i = 0; i < Old.Items.Num(); ++i)
	{
		if (Old.Items[i] != InventoryItems.Items[i])
		{
			if (InventoryItems.Items[i])
			{
				ItemUpdated.Add(InventoryItems.Items[i]);
			}
		}
	}

	if (ItemUpdated.Num() > 0)
	{
		FIntPoint Dimension = ItemUpdated[0]->GetDimentions();
		int Size = Dimension.X * Dimension.Y;

		if (Size == ItemUpdated.Num())
		{
			auto Pawn = Cast<APawn>(GetOwner());
			if (Pawn && Pawn->IsPlayerControlled() && Pawn->IsLocallyControlled())
			{
				UGameplayStatics::PlaySound2D(GetOwner(), ItemUpdated[0]->GetInventorySound());
			}
			else
			{
				Pawn = Cast<APawn>(InventoryItems.InteractingActor);
				if (Pawn && Pawn->IsPlayerControlled() && Pawn->IsLocallyControlled())
				{
					UGameplayStatics::PlaySound2D(GetOwner(), ItemUpdated[0]->GetInventorySound());
				}
			}
		}
	}
}

inline TTuple<bool, UItemObject*> UPlayerEquipmentComponent::GetItematIndex(int32 Index) const
{
	if (ensureAlways(InventoryItems.Items.IsValidIndex(Index)))
		return MakeTuple(IsValid(InventoryItems.Items[Index]), InventoryItems.Items[Index]);
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
