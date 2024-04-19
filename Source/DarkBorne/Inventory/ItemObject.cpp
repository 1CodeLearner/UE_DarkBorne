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
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	//return _dimentions;
	FIntPoint Temp(Item.SlotHolder.SlotDimension.X, Item.SlotHolder.SlotDimension.Y);
	return Temp;
}

UMaterialInterface* UItemObject::GetIcon()
{
	return nullptr; //Item.SlotHolder.DisplayMaterial;
}

TSubclassOf<AActor> UItemObject::GetItemClass()
{
	return TSubclassOf<AActor>();//Item.SlotHolder.ItemClass;
}

ESlotType UItemObject::GetSlotType() const
{
	return ESlotType::NONE;//Item.SlotHolder.SlotType;
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
			*Item.SlotHolder.DisplayName.ToString()
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