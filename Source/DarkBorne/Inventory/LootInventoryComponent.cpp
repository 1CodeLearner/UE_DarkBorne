// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInventoryComponent.h"
#include "../Framework/EntityTypes.h"
#include "../DBCharacters/DBCharacter.h"
#include "InventoryMainWidget.h"
ULootInventoryComponent::ULootInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULootInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = GetOwner<ADBCharacter>();
} 

void ULootInventoryComponent::DisplayLoot(AActor* OtherActor, EEntityType EntityType)
{
	switch (EntityType)
	{
	case EEntityType::PLAYER:
		if (ensureAlways(Character)) 
		{
			
			Character->InvMainWidget->StartInit(EntityType);
		}
		break;
	case EEntityType::ENEMY:
		break;
	default:
		return;
	}
	
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

