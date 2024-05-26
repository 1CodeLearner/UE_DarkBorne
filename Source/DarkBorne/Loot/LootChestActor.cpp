// Fill out your copyright notice in the Description page of Project Settings.


#include "LootChestActor.h"
#include "../Inventory/LootInventoryComponent.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/InventoryMainWidget.h"
#include "Components/StaticMeshComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "../Inventory/ItemObject.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

ALootChestActor::ALootChestActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	LootInventoryComp = CreateDefaultSubobject<ULootInventoryComponent>("LootInventoryComponent");

	SMComp_ChestBase = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent_Base");
	RootComponent = SMComp_ChestBase;
	SMComp_ChestBase->SetCollisionProfileName("Item");

	SMComp_ChestLid = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent_Lid");
	SMComp_ChestLid->SetupAttachment(RootComponent);
	SMComp_ChestLid->SetCollisionProfileName("Item");
	SMComp_ChestLid->SetRelativeLocation(FVector(0.f,-35.f,45.f));

	AudioComp_Looting = CreateDefaultSubobject<UAudioComponent>("AudioComponent_Looting");
	AudioComp_Looting->SetupAttachment(RootComponent);
	AudioComp_Looting->SetAutoActivate(false);

	AudioComp_Open = CreateDefaultSubobject<UAudioComponent>("AudioComponent_Open");
	AudioComp_Open->SetupAttachment(RootComponent);
	AudioComp_Open->SetAutoActivate(false);

	bIsOpened = false;
}

void ALootChestActor::BeginPlay()
{
	Super::BeginPlay();
	SetInvenEquipType(EInvenEquipType::Chest);
}

void ALootChestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootChestActor, bIsOpened);
}

void ALootChestActor::OnRep_bIsOpened()
{
	if (bIsOpened)
	{
		if (SMComp_ChestLid)
		{
			SMComp_ChestLid->SetRelativeRotation(FRotator(0.f, 0.f, -100.f));
		}
		SetInteractionTime(27.f);
	}
}

void ALootChestActor::BeginInteract(UDBInteractionComponent* InteractionComp)
{
	SMComp_ChestLid->SetRenderCustomDepth(false);
	SMComp_ChestBase->SetRenderCustomDepth(false);
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
		if (!bIsOpened)
		{
			AudioComp_Open->Play();
		}
	}

	if (HasAuthority() && !bIsOpened)
	{
		bIsOpened = true;
		OnRep_bIsOpened();
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
	SMComp_ChestLid->SetRenderCustomDepth(true);
	SMComp_ChestBase->SetRenderCustomDepth(true);
}

void ALootChestActor::EndTrace()
{
	SMComp_ChestLid->SetRenderCustomDepth(false);
	SMComp_ChestBase->SetRenderCustomDepth(false);
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

