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

void ULootInventoryComponent::DisplayLoot(AActor* OtherActor, EEntityType EntityType)
{
	if (!ensureAlways(LootDisplayWidget)) return;
	Server_CopyItems(OtherActor, EntityType);
}

void ULootInventoryComponent::Server_CopyItems_Implementation(AActor* OtherActor, EEntityType EntityType)
{
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

