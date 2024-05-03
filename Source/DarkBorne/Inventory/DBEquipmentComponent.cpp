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
	SetIsReplicatedByDefault(true);
	bIsDirty = false;
	bInvalidSlot = false;
	Columns = 2;
	Rows = 2;
}

bool UDBEquipmentComponent::TryAddItem(UItemObject* ItemObject)
{
	if (!IsValid(ItemObject) && Slots.IsEmpty())
		return false;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	if (Slots[index] != nullptr)
	{
		return false;
	}
	else {
		Server_AddItem(ItemObject);
		return true;
	}
}

void UDBEquipmentComponent::Server_AddItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Slots;
	if (Slots.IsEmpty()) return;
	Slots[index] = ItemObject;

	UE_LOG(LogTemp, Warning, TEXT("Actor %s"), *GetNameSafe(ItemObject->GetItemActor()));

	//auto WeaponComp = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	//if (WeaponComp)
	//{	
	//	WeaponComp->PassItem(ItemObject);
	//}
	OnRep_What(old);

	ItemObject->TryDestroyItemActor();
}

void UDBEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Slots;
	Slots[index] = nullptr;
	OnRep_What(old);
}


const TArray<UItemObject*> UDBEquipmentComponent::GetSlots() const
{
	return Slots;
}

const UItemObject* UDBEquipmentComponent::GetSlotItem(ESlotType SlotType) const
{
	int32 index = UItemLibrary::GetSlotIndexByEnum(SlotType);
	if (Slots.IsEmpty()) return nullptr;
	return Slots[index];
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
			Slots.SetNum(size, false);

			int val = 10;
		}
	}
}

void UDBEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDirty)
	{
		bIsDirty = false;
		OnEquipmentChanged.Broadcast();
	}

	if (Slots[0])
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("EquipComp: %s [%s]: %s, Num:%d"),
			*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
			*Slots[0]->GetItem().SlotHolder.DisplayName.ToString(), Slots.Num())
		);
	}

}

bool UDBEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UItemObject* ItemObject : Slots)
	{
		if (ItemObject)
		{
			WroteSomething |= Channel->ReplicateSubobject(ItemObject, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UDBEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBEquipmentComponent, Slots);
}

void UDBEquipmentComponent::OnRep_What(TArray<UItemObject*> OldSlots)
{
	bIsDirty = true;
	//if (!OldSlots.IsEmpty() && OldSlots[0])
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnRep_What Old: s%, new: s%"),
	//		*OldSlots[0]->GetItem().SlotHolder.DisplayName.ToString(),
	//		*Slots[0]->GetItem().SlotHolder.DisplayName.ToString()
	//	);
	//}
	//else if (!Slots.IsEmpty() && Slots[0])
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnRep_What Old: empty, new: s%"),
	//		*Slots[0]->GetItem().SlotHolder.DisplayName.ToString()
	//	);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnRep_What Initializing"));
	//}

}

