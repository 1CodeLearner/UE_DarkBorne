// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySoundComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "InventorySoundComponent.h"
#include "ItemObject.h"

UInventorySoundComponent::UInventorySoundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventorySoundComponent::DeclareSoundType(EInvenEquipType Type, UItemObject* ItemObject)
{
	if(!ItemObject)
	{
		return;
	}

	auto Pawn = Cast<APawn>(GetOwner());
	if (Pawn && Pawn->IsLocallyControlled())
	{
		switch (Type)
		{
		case EInvenEquipType::Equip:
		{
			UE_LOG(LogTemp, Error, TEXT("DeclareSoundType : Equip"));
			break;
		}
		case EInvenEquipType::Inventory:
		{
			UE_LOG(LogTemp, Error, TEXT("DeclareSoundType : Inventory"));
			break;
		}
		case EInvenEquipType::Spawn:
		{
			UE_LOG(LogTemp, Error, TEXT("DeclareSoundType : Spawn"));
			break;
		}
		}
	}
}