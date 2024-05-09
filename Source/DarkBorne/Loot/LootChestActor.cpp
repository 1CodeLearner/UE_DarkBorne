// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestActor.h"
#include "../Inventory/LootInventoryComponent.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/InventoryMainWidget.h"
#include "Components/StaticMeshComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "../Inventory/ItemObject.h"

ALootChestActor::ALootChestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	LootInventoryComp = CreateDefaultSubobject<ULootInventoryComponent>("LootInventoryComp");
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	RootComponent = SMComp;
	SMComp->SetCollisionProfileName("Item");
}

void ALootChestActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALootChestActor::BeginInteract(UDBInteractionComponent* InteractionComp)
{
}

void ALootChestActor::ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character)
{
	auto OtherPlayer = Cast<ADBCharacter>(Character);
	if (ensureAlways(OtherPlayer) && OtherPlayer->InvMainWidget)
	{
		OtherPlayer->InvMainWidget->InitLootDisplay(this);
		if (OtherPlayer->InvMainWidget->IsLootValid())
		{
			OtherPlayer->InvMainWidget->DisplayInventory(true);
		}
	}
}

void ALootChestActor::InterruptInteract()
{
}

void ALootChestActor::BeginTrace()
{
}

void ALootChestActor::EndTrace()
{

}

bool ALootChestActor::CanInteract() const
{
	return true;
}

void ALootChestActor::SetCanInteract(bool bAllowInteract)
{
	//empty
}

FDisplayInfo ALootChestActor::GetDisplayInfo() const
{
	return FDisplayInfo(TEXT("Loot"), TEXT("Chest"));
}

void ALootChestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

