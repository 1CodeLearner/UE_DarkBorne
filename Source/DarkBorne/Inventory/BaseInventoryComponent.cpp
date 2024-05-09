// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "../Inventory/ItemObject.h"
#include "../Items/DBItem.h"
#include "Kismet/GameplayStatics.h"

UBaseInventoryComponent::UBaseInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	bIsDirty = false;
}

void UBaseInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsDirty)
	{
		bIsDirty = false;
		OnInventoryChanged.Broadcast();
	}
}

void UBaseInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseInventoryComponent, Items);
}

bool UBaseInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UItemObject* ItemObject : Items)
	{
		WroteSomething |= Channel->ReplicateSubobject(ItemObject, *Bunch, *RepFlags);
	}
	return WroteSomething;
}

bool UBaseInventoryComponent::TryAddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	return false;
}
void UBaseInventoryComponent::RemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer)
{
	return;
}

float UBaseInventoryComponent::GetTileSize() const
{
	return TileSize;
}

FVector2D UBaseInventoryComponent::GetSize() const
{
	int32 x = Columns * TileSize;
	int32 y = Rows * TileSize;
	return FVector2D(x, y);
}

void UBaseInventoryComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject)
{
	return;
}

void UBaseInventoryComponent::Server_SpawnItem_Implementation(AActor* Initiator, UItemObject* ItemObject, bool bSetOwner, float forwardOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("Net? %s"),
		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER")
	);
	if (Initiator && ItemObject) {
		FTransform Trans = GetNewTransform(Initiator, forwardOffset);

		auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(ItemObject->GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);

		ItemSpawned->Initialize(ItemObject);

		ItemObject->SetItemActor(ItemSpawned);

		UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Item Failed in %s"), *GetNameSafe(this));
		return;
	}

}

FTransform UBaseInventoryComponent::GetNewTransform(AActor* Instigator, float offset)
{
	FVector SpawnLoc = Instigator->GetActorLocation();
	SpawnLoc += Instigator->GetActorForwardVector() * offset;

	FTransform Trans;

	Trans.SetLocation(SpawnLoc);
	Trans.SetRotation(FQuat::Identity);
	Trans.SetScale3D(FVector::OneVector);

	return Trans;
}

void UBaseInventoryComponent::OnRep_Items(TArray<UItemObject*> OldItemArray)
{
	bIsDirty = true;
}