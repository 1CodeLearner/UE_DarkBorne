// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestActor.h"
#include "../Inventory/LootInventoryComponent.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/InventoryMainWidget.h"
#include "Components/StaticMeshComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "../Inventory/ItemObject.h"
#include "Components/AudioComponent.h"

ALootChestActor::ALootChestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	LootInventoryComp = CreateDefaultSubobject<ULootInventoryComponent>("LootInventoryComp");
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	RootComponent = SMComp;
	SMComp->SetCollisionProfileName("Item");

	AudioComp_Looting = CreateDefaultSubobject<UAudioComponent>("AudioComponent_Looting");
	AudioComp_Looting->SetupAttachment(RootComponent);
	AudioComp_Looting->SetAutoActivate(false);
	AudioComp_Open = CreateDefaultSubobject<UAudioComponent>("AudioComponent_Open");
	AudioComp_Open->SetupAttachment(RootComponent);
	AudioComp_Open->SetAutoActivate(false);
}

void ALootChestActor::BeginPlay()
{
	Super::BeginPlay();
	SetInvenEquipType(EInvenEquipType::Chest);
}

void ALootChestActor::BeginInteract(UDBInteractionComponent* InteractionComp)
{
	SMComp->SetRenderCustomDepth(false);
	if (AudioComp_Looting && !AudioComp_Looting->IsPlaying())
	{
		AudioComp_Looting->Play();
	}
}

void ALootChestActor::ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character)
{
	auto OtherPlayer = Cast<ADBCharacter>(Character);
	if (OtherPlayer && OtherPlayer->IsLocallyControlled() && OtherPlayer->InvMainWidget && !OtherPlayer->InvMainWidget->IsInViewport())
	{
		OtherPlayer->InvMainWidget->InitLootDisplay(this);
		if (OtherPlayer->InvMainWidget->IsLootValid())
		{
			OtherPlayer->DisplayInventory(true);
		}
	}

	if (AudioComp_Looting && AudioComp_Looting->IsPlaying() && AudioComp_Open && !AudioComp_Open->IsPlaying())
	{
		AudioComp_Looting->Stop();
		AudioComp_Open->Play();
	}
}

void ALootChestActor::InterruptInteract()
{
	if (AudioComp_Looting && AudioComp_Looting->IsPlaying())
	{
		AudioComp_Looting->Stop();
	}
}

void ALootChestActor::BeginTrace()
{
	SMComp->SetRenderCustomDepth(true);
}

void ALootChestActor::EndTrace()
{
	SMComp->SetRenderCustomDepth(false);
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

