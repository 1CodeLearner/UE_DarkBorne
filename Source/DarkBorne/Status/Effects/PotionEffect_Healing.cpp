// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionEffect_Healing.h"
#include "../../Inventory/ItemObject.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../CharacterStatusComponent.h"


UPotionEffect_Healing::UPotionEffect_Healing()
{
	Amount = 15.f;
	prevHealAmount = 0.f;
	healTickRate = 2.5f;
	nextHealTick = healTickRate;
}

void UPotionEffect_Healing::Initialize(ADBCharacter* Instigator, UItemObject* Item)
{	
	Super::Initialize(Instigator, Item);
	TotalTime = Item->GetRarityValue();
}

void UPotionEffect_Healing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	currTime += DeltaTime;
	if (currTime > nextHealTick)
	{
		nextHealTick += healTickRate;
		auto StatusComp = AffectedCharacter->GetComponentByClass<UCharacterStatusComponent>();
		if (ensureAlways(StatusComp)) 
		{
			StatusComp->
		}
	}
	float currHealAmount = Amount * (currTime / TotalTime);
	
}
