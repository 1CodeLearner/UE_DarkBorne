// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffectComponent.h"
#include "DBEffect.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/ItemObject.h"
#include "../Items/Consumables/DBConsumable.h"

UDBEffectComponent::UDBEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDBEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDBEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDBEffectComponent::AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate)
{
	if (ItemToActivate) 
	{
		auto NewEffect = NewObject<UDBEffect>(Instigated, ItemToActivate->GetEffectClass());
		NewEffect->Initialize(Instigated, ItemToActivate->GetItemObject());
		Effects.Add(NewEffect);

		OnInitStart.ExecuteIfBound(NewEffect, ItemToActivate->GetItemObject()->GetIcon());

		NewEffect->StartTick();
	}
}

void UDBEffectComponent::RemoveEffect(UDBEffect* Effect)
{
	Effect->StopTick();
	Effects.RemoveSingle(Effect);
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