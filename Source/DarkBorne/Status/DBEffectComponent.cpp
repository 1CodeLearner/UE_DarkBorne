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
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Effects:%d"),Effects.Num()));
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

		OnInitStart.ExecuteIfBound(NewEffect, ItemToActivate->GetItemObject()->GetIcon());

		NewEffect->StartTick();
	}
}

void UDBEffectComponent::RemoveEffect(UDBEffect* Effect)
{
	Effect->StopTick();
	Effects.RemoveSingle(Effect);
}

void UDBEffectComponent::RemoveAllEffects()
{
	for(auto Effect : Effects)
	{
		Effect->StopTick();
	}
	Effects.Empty();
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