// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "PlayerEquipmentComponent.h"
#include "ItemObject.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include "PlayerEquipmentComponent.h"
#include "../DBCharacters/DBRogueCharacter.h"
#include "../DBPlayerWidget/DBPlayerWidget.h"

UDBEquipmentComponent::UDBEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bInvalidSlot = false;
	bOccupiedSlot = false;
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

void UDBEquipmentComponent::ProcessActiveItem(UItemObject* ItemObject)
{
}

void UDBEquipmentComponent::OnRep_Items(TArray<UItemObject*> OldItemArray)
{
	Super::OnRep_Items(OldItemArray);

	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	if(RoguePlayer->PlayerWidget)
		RoguePlayer->PlayerWidget->UpdateSlot(GetSlots());
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
		AddItem(ItemObject, TaxiToServer);
		return true;
	}
}

void UDBEquipmentComponent::RemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	if (!IsValid(ItemObject)) return;
	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	if (ItemObject != Items[index])
		return;

	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/

	if (this->GetOwner()->HasNetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
		Server_RemoveItem(ItemObject);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
		auto EquipCompTaxi = Cast<UDBEquipmentComponent>(TaxiToServer);
		EquipCompTaxi->Server_TaxiForRemoveItem(ItemObject, this);
	}
}

void UDBEquipmentComponent::Server_TaxiForRemoveItem_Implementation(UItemObject* ItemObject, UBaseInventoryComponent* TaxiedInventoryComp)
{
	auto TaxiedEquipComp = Cast<UDBEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedEquipComp))
	{
		TaxiedEquipComp->Server_RemoveItem(ItemObject);
	}
}

void UDBEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> old = Items;
	UItemObject* TempItemObj = Items[index];
	Items[index] = nullptr;

	auto WeaponComp = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	if (WeaponComp)
	{
		WeaponComp->TryRemoveRogueItem(ItemObject);
	}

	OnRep_Items(old);
}

void UDBEquipmentComponent::AddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	if (!IsValid(ItemObject)) return;
	/*if (!ensureAlwaysMsgf(TaxiToServer->GetOwner()->HasNetOwner() ||
		this->GetOwner()->HasNetOwner(), TEXT("ensure this function has a reference to object that has owning connection for RPC call")))
		return;*/

	if (this->GetOwner()->HasNetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Has NetOwner()"));
		Server_AddItem(ItemObject);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Has no NetOwner()"));
		auto EquipCompTaxi = Cast<UDBEquipmentComponent>(TaxiToServer);
		EquipCompTaxi->Server_TaxiForAddItem(ItemObject, this);
	}
}

void UDBEquipmentComponent::Server_TaxiForAddItem_Implementation(UItemObject* ItemObject, UBaseInventoryComponent* TaxiedInventoryComp)
{
	auto TaxiedEquipComp = Cast<UDBEquipmentComponent>(TaxiedInventoryComp);
	if (ensureAlways(TaxiedEquipComp))
	{
		TaxiedEquipComp->Server_AddItem(ItemObject);
	}
}

void UDBEquipmentComponent::Server_AddItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);

	TArray<UItemObject*> old = Items;
	if (Items.IsEmpty())
		return;

	if (Items[index])
	{
		UItemObject* TempItemObj = Items[index];
		auto PEComp = GetOwner()->GetComponentByClass<UPlayerEquipmentComponent>();
		//Throw Item away if inventory is full
		if (!PEComp->TryAddItem(TempItemObj, PEComp))
		{
			Server_SpawnItem(GetOwner(), TempItemObj);
		}
	}

	Items[index] = ItemObject;

	ItemObject->TryDestroyItemActor();

	auto WeaponComp = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	if (WeaponComp)
	{
		WeaponComp->PassItem(ItemObject);
	}

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

bool UDBEquipmentComponent::IsSlotVacant(UItemObject* ItemObject) const
{
	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	if (Items.IsValidIndex(index) && Items[index])
		return false;

	return true;
}

