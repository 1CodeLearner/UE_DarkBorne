// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEquipmentComponent.h"
#include "../ItemTypes/ItemType.h"
#include "PlayerEquipmentComponent.h"
#include "ItemObject.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "Net/UnrealNetwork.h"

UDBEquipmentComponent::UDBEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UDBEquipmentComponent::Server_AddItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> Obj = Slots;
	Slots[index] = ItemObject;
	//OnRep_What(Obj);
}

void UDBEquipmentComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	if (!ensureAlways(ItemObject))
		return;

	int32 index = UItemLibrary::GetSlotIndexByObject(ItemObject);
	TArray<UItemObject*> Obj = Slots;
	Slots[index] = nullptr;
	//OnRep_What(Obj);
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

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("%s [%s]: %s, Num:%d"), 
	*GetNameSafe(GetOwner()), (GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")), *GetNameSafe(Slots[0]), Slots.Num())
	);
}

void UDBEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBEquipmentComponent, Slots);
}

void UDBEquipmentComponent::OnRep_What(TArray<UItemObject*> OldSlots)
{
	if (!OldSlots.IsEmpty())
		UE_LOG(LogTemp, Warning, TEXT("Old: s%, new: s%"), *GetNameSafe(OldSlots[0]), *GetNameSafe(Slots[0]));
}

