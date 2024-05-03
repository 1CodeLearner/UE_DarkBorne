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
	isDirty = false;
	Columns = 8;
	Rows = 5;
}

void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	itemArray.SetNum(Columns * Rows);
}

void UPlayerEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerEquipmentComponent, itemArray);
	
	

}

bool UPlayerEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for(UItemObject* ItemObject: itemArray)
	{
		WroteSomething |= Channel->ReplicateSubobject(ItemObject, *Bunch, *RepFlags);
	}
	return WroteSomething;
}

void UPlayerEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (isDirty)
	{
		isDirty = false;
		onInventoryChangedDel.Broadcast();
	}
}

bool UPlayerEquipmentComponent::TryAddItem(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject)) return false;
	for (int i = 0; i < itemArray.Num(); i++)
	{
		if (IsRoomAvailable(ItemObject, i))
		{
			Server_AddItemAt(ItemObject, i);
			return true;
		}
		else continue;
	}
	return false;
}

void UPlayerEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject)) return;
	TArray<UItemObject*> old = itemArray;
	for (int32 i = 0; i < itemArray.Num(); i++)
	{
		if (itemArray[i] == ItemObject)
		{
			itemArray[i] = nullptr;
		}
	}
	OnRep_itemArray(old);
}

void UPlayerEquipmentComponent::Server_AddItemAt_Implementation(UItemObject* ItemObject, int32 TopLeftIndex)
{
	////ForEachIndex
	FTile refTile = IndexToTile(TopLeftIndex);
	FIntPoint dimentions = ItemObject->GetDimentions();
	FTile newTile;
	TArray<UItemObject*> old = itemArray;
	for (int32 i = refTile.X; i < refTile.X + (dimentions.X); i++)
	{
		for (int32 j = refTile.Y; j < refTile.Y + (dimentions.Y); j++)
		{
			newTile.X = i;
			newTile.Y = j;
			itemArray[TileToIndex(newTile)] = ItemObject;
		}
	}

	ItemObject->TryDestroyItemActor();
	OnRep_itemArray(old);
}

TMap<class UItemObject*, FTile> UPlayerEquipmentComponent::GetAllItems() const
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
				TTuple<bool, UItemObject*> output = GetItematIndex(num);
				bool valid = output.Get<0>();
				UItemObject* outItemObject = output.Get<1>();
				if (valid)
				{
					if (IsValid(outItemObject))
					{
						return false;
					}
				}
				//else return false;
			}
			else return false;
		}
	}
	return true;

}

inline TTuple<bool, UItemObject*> UPlayerEquipmentComponent::GetItematIndex(int32 Index) const
{
	return MakeTuple(IsValid(itemArray[Index]), itemArray[Index]);
}

bool UPlayerEquipmentComponent::IsTileValid(FTile tile) const
{
	if (tile.X >= 0 && tile.Y >= 0 && tile.X < Columns && tile.Y < Columns)
	{
		return true;
	}

	return false;

}

void UPlayerEquipmentComponent::OnRep_itemArray(TArray<UItemObject*> OldItemArray)
{
	isDirty = true;
}

void UPlayerEquipmentComponent::Server_SpawnItem_Implementation(AActor* Initiator, UItemObject* ItemObject, bool bSetOwner, float forwardOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("Net? %s"),
		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER")
	);
	if (Initiator && ItemObject) {
		FTransform Trans = GetNewTransform(Initiator, forwardOffset);

		auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(ItemObject->GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);

		ItemSpawned->Initialize(ItemObject);

		ItemObject->AddItemActor(ItemSpawned);

		UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Item Failed in %s"), *GetNameSafe(this));
		return;
	}
}

void UPlayerEquipmentComponent::Server_PlaceItem_Implementation(AActor* Initiator, UItemObject* ItemObject, bool bSetOwner, float forwardOffset)
{
	AActor* actor = Cast<AActor>(ItemObject->GetItemActor());
	if (ensureAlways(actor)) 
	{
		FTransform Trans = GetNewTransform(Initiator, forwardOffset);
		actor->SetActorTransform(Trans);
	}
}

FTransform UPlayerEquipmentComponent::GetNewTransform(AActor* Instigator, float offset)
{
	FVector SpawnLoc = Instigator->GetActorLocation();
	SpawnLoc += Instigator->GetActorForwardVector() * offset;

	FTransform Trans;
	
	Trans.SetLocation(SpawnLoc);
	Trans.SetRotation(FQuat::Identity);
	Trans.SetScale3D(FVector::OneVector);

	return Trans;
}
