// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "../Items/PDA_ItemSlot.h"


// Sets default values for this component's properties
UPlayerEquipmentComponent::UPlayerEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	
	
}


// Called every frame
void UPlayerEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayerEquipmentComponent::TryAddItem(FItem ItemObject)
{
	return false;
}

//TMap<FItem, FTile> UPlayerEquipmentComponent::GetAllItems()
//{
//	return TMap<FItem, FTile>();
//}

FTile UPlayerEquipmentComponent::IndexToTile(int32 Index)
{
	return FTile();
}

int32 UPlayerEquipmentComponent::TileToIndex(struct FTile Tile)
{
	return -1;
}

bool UPlayerEquipmentComponent::AddItemAt(FItem ItemObject, int32 TopLeftIndex)
{
	return false;
}

bool UPlayerEquipmentComponent::IsRoomAvailable(FItem ItemObject, int32 TopLeftIndex)
{
	return false;
}

bool UPlayerEquipmentComponent::RemoveItem(FItem ItemObject)
{
	return false;
}

