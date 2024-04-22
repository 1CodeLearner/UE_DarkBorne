// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInventoryComponent.h"
#include "../Framework/EntityTypes.h"

ULootInventoryComponent::ULootInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULootInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULootInventoryComponent::CopyItems(UPlayerEquipmentComponent* OtherInventory, EEntityType EntityType)
{
	AActor* Owner = OtherInventory->GetOwner();

	switch (EntityType)
	{
	case EEntityType::PLAYER:
		break;
	case EEntityType::ENEMY:
		break;
	default: 
		return;
	}
}


void ULootInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

