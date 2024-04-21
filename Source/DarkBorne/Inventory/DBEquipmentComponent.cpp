// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"

#include "PlayerEquipmentComponent.h"
#include "ItemObject.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UDBEquipmentComponent::UDBEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bIsDirty = false;
}

void UDBEquipmentComponent::Server_AddItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Slots;
	Slots[index] = ItemObject;
	OnRep_What(old);
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


void UDBEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	ESlotType slotNum = ESlotType::NONE;
	int32 size = (int32)slotNum - 1;
	Slots.SetNum(size, false);
	if (GetOwner())
	{
		auto PEC = GetOwner()->GetComponentByClass<UPlayerEquipmentComponent>();
		if (ensureAlways(PEC))
			PlayerEquipComp = PEC;
	}
}

void UDBEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Slots[0])
	{
		if (!GetOwner()->HasAuthority())
		{
			UPDA_ItemSlot* Test = Slots[0]->GetItem().ItemSlot;
			if (Test)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("EquipComp: %s [%s]: %s, Num:%d"),
					*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")), 
					*Slots[0]->GetItem().SlotHolder.DisplayName.ToString(), Slots.Num())
				);
			}
			else
				GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("EquipComp: %s [%s]: %s, Num:%d"),
					*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")), 
					TEXT("Invalid UPDA_ItemSlot"), 
					Slots.Num())
				);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("EquipComp: %s [%s]: %s, Num:%d"),
				*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")), 
				*Slots[0]->GetItem().SlotHolder.DisplayName.ToString(), 
				Slots.Num())
			);
		}

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

