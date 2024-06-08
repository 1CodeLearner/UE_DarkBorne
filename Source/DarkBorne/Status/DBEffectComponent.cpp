// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffectComponent.h"
#include "DBEffect.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/ItemObject.h"
#include "../Items/Consumables/DBConsumable.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UDBEffectComponent::UDBEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UDBEffectComponent::BeginPlay()
{
	Super::BeginPlay();

}
void UDBEffectComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(UDBEffectComponent, Effects, COND_OwnerOnly);
	DOREPLIFETIME(UDBEffectComponent, Effects);

}

bool UDBEffectComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UDBEffect* Effect : Effects)
	{
		if (Effect)
		{
			WroteSomething |= Channel->ReplicateSubobject(Effect, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UDBEffectComponent::AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate)
{
	Server_AddEffect(Instigated, ItemToActivate);
}

void UDBEffectComponent::Server_AddEffect_Implementation(ADBCharacter* Instigated, ADBConsumable* ItemToActivate)
{
	if (ItemToActivate)
	{
		auto NewEffect = NewObject<UDBEffect>(Instigated, ItemToActivate->GetEffectClass());
		NewEffect->Initialize(Instigated, ItemToActivate->GetItemObject(), this);
		Effects.Add(NewEffect);

		OnRep_Effects();

		//OnEffectStart.ExecuteIfBound(NewEffect, ItemToActivate->GetItemObject()->GetIcon());
		NewEffect->StartTick();
	}
}

void UDBEffectComponent::RemoveEffect(UDBEffect* Effect)
{
	Effect->StopTick();
	Effects.RemoveSingle(Effect);

	OnRep_Effects();
}

void UDBEffectComponent::RemoveAllEffects()
{
	for (auto Effect : Effects)
	{
		Effect->StopTick();
	}
	Effects.Empty();

	OnRep_Effects();
}

bool UDBEffectComponent::CanStartEffect(ADBConsumable* ItemToActivate)
{
	if (!ItemToActivate) return false;

	UItemObject* ItemObject = ItemToActivate->GetItemObject();

	for (int32 i = 0; i < Effects.Num(); ++i)
	{
		if (Effects[i]->IsSame(ItemObject->GetId()))
			return false;
	}

	return true;
}

void UDBEffectComponent::OnRep_Effects()
{
	OnEffectUpdate.ExecuteIfBound(Effects);
}
