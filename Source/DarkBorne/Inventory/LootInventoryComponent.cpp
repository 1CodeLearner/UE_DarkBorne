// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInventoryComponent.h"
#include "../Framework/EntityTypes.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"

ULootInventoryComponent::ULootInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void ULootInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		auto GameMode = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
		if (ensureAlways(GameMode) && ensureAlways(!DropName.IsNone()))
			GameMode->AddItemTo(GetOwner(), DropName);
	}
}
