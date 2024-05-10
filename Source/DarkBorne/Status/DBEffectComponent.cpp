// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffectComponent.h"
#include "DBEffect.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/ItemObject.h"

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

void UDBEffectComponent::AddEffect(ADBCharacter* Instigated, TSubclassOf<UDBEffect> EffectClass, UItemObject* ItemObject)
{
	auto NewEffect = NewObject<UDBEffect>(Instigated, EffectClass);
	NewEffect->Initialize(Instigated, ItemObject);
	Effects.Add(NewEffect);

	OnInitStart.ExecuteIfBound(NewEffect, ItemObject->GetIcon());
	
	NewEffect->StartTick();
}

void UDBEffectComponent::RemoveEffect(UDBEffect* Effect)
{
	Effect->StopTick();
	Effects.RemoveSingle(Effect);
}

bool UDBEffectComponent::CanStartEffect(UDBEffect* Effect)
{
	for (int32 i = 0; i < Effects.Num(); ++i) 
	{
		if(Effects[i]->IsSame(Effect))
			return false;
	}

	return true;
}