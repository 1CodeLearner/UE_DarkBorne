// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "../Items/PDA_ItemSlot.h"
#include "Materials/MaterialInterface.h"
#include "../Items/DBItem.h"
#include "Net/UnrealNetwork.h"

void UItemObject::Initialize(FItem item)
{
	this->Item = item;
}

FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // ���� ������ 100x100 ����
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

ESlotType UItemObject::GetSlotType() const
{
	return Item.ItemSlot->SlotType;
}

const FItem& UItemObject::GetItem() const
{
	return Item;
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

TStatId UItemObject::GetStatId() const
{
	return TStatId();
}

void UItemObject::Tick(float DeltaTime)
{
	if (Item.ItemSlot)
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf
		(
			TEXT("[%s] %s"),
			(GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
			*Item.ItemSlot->DisplayName.ToString()
		)
		);
	else if (GetWorld())
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf
		(
			TEXT("[%s] UPDA_ItemSlot null"),
			(GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"))
		)
		);
}