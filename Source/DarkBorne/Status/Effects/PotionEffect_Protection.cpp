// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionEffect_Protection.h"
#include "../../Inventory/ItemObject.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../DBEffectComponent.h"
#include "../CharacterStatusComponent.h"

UPotionEffect_Protection::UPotionEffect_Protection()
{
	TotalTime = 24.f;
}

void UPotionEffect_Protection::Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp)
{
	Super::Initialize(Instigator, Item, EffectComp);

	float RarityValue = Item->GetRarityValue();
	Amount = RarityValue;
}

void UPotionEffect_Protection::StartTick()
{
	Super::StartTick();

	auto StatusComp = UCharacterStatusComponent::Get(AffectedCharacter);
	if (StatusComp)
	{
		StatusComp->AddBlockAmount(Amount);
	}

}

void UPotionEffect_Protection::StopTick()
{
	Super::StopTick();

	auto StatusComp = UCharacterStatusComponent::Get(AffectedCharacter);
	if (StatusComp)
	{
		StatusComp->RemoveBlockAmount(Amount);
	}
}
