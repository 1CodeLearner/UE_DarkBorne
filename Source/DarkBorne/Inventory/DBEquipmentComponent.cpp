// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "PlayerEquipmentComponent.h"
#include "ItemObject.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../DBWeapon/DBRogueWeaponComponent.h"

UDBEquipmentComponent::UDBEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bInvalidSlot = false;
	Columns = 2;
	Rows = 2;
}

void UDBEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		auto PEC = GetOwner()->GetComponentByClass<UPlayerEquipmentComponent>();
		if (ensureAlways(PEC))
			PlayerEquipComp = PEC;

		if (GetOwner()->HasAuthority())
		{
			ESlotType slotNum = ESlotType::NONE;
			int32 size = (int32)slotNum - 1;
			Items.SetNum(size, false);

			int val = 10;
		}
	}
}

void UDBEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Items[0])
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("EquipComp: %s [%s]: %s, Num:%d"),
			*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
			*Items[0]->GetItem().SlotHolder.DisplayName.ToString(), Items.Num())
		);
	}
}

bool UDBEquipmentComponent::TryAddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	if (!IsValid(ItemObject) && Items.IsEmpty())
		return false;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	if (Items[index] != nullptr)
	{
		return false;
	}
	else {
		Server_AddItem(ItemObject);
		return true;
	}
}

void UDBEquipmentComponent::RemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	Server_RemoveItem(ItemObject);
}

void UDBEquipmentComponent::Server_AddItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Items;
	if (Items.IsEmpty()) return;
	Items[index] = ItemObject;

	UE_LOG(LogTemp, Warning, TEXT("Actor %s"), *GetNameSafe(ItemObject->GetItemActor()));

	//auto WeaponComp = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	//if (WeaponComp)
	//{	
	//	WeaponComp->PassItem(ItemObject);
	//}
	OnRep_Items(old);

	ItemObject->TryDestroyItemActor();
}

void UDBEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Items;
	Items[index] = nullptr;
	OnRep_Items(old);
}

const TArray<UItemObject*> UDBEquipmentComponent::GetSlots() const
{
	return Items;
}

const UItemObject* UDBEquipmentComponent::GetSlotItem(ESlotType SlotType) const
{
	int32 index = UItemLibrary::GetSlotIndexByEnum(SlotType);
	if (Items.IsEmpty()) return nullptr;
	return Items[index];
}

