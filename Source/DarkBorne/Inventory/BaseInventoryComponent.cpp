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

	DOREPLIFETIME(UBaseInventoryComponent, InventoryItems);
}

bool UBaseInventoryComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UItemObject* ItemObject : InventoryItems.Items)
	{
		WroteSomething |= Channel->ReplicateSubobject(ItemObject, *Bunch, *RepFlags);
	}
	return WroteSomething;
}

bool UBaseInventoryComponent::HasItem(UItemObject* ItemObject) const
{
	return false;
}

void UBaseInventoryComponent::ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	return;
}

void UBaseInventoryComponent::Server_ProcessPressInput_Implementation(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	return;
}

void UBaseInventoryComponent::Server_TaxiForProcessPressInput_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput)
{
	return;
}

bool UBaseInventoryComponent::TryAddItem(UItemObject* ItemObject, AActor* InitiatedActor)
{
	return false;
}
void UBaseInventoryComponent::RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor)
{
	return;
}

void UBaseInventoryComponent::Server_RemoveItem_Implementation(UItemObject* ItemObject, AActor* InitiatedActor)
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
		ItemObject->TryDestroyItemActor();
		ItemObject->SpawnItem(Initiator, bSetOwner, Trans, forwardOffset);
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
	FRotator Rotation = Instigator->GetActorRotation();
	float PitchRand = FMath::RandRange(-30.f, 30.f);
	float RollRand = FMath::RandRange(-30.f, 30.f);
	Rotation.Pitch += PitchRand;
	Rotation.Roll += RollRand;
	Trans.SetRotation(Rotation.Quaternion());
	Trans.SetScale3D(FVector::OneVector);

	return Trans;
}

void UBaseInventoryComponent::OnRep_Items(FInventoryItems Old)
{
	bIsDirty = true;
}