// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionEffect_Healing.h"
#include "../../Inventory/ItemObject.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../CharacterStatusComponent.h"
#include "../DBEffectComponent.h"

UPotionEffect_Healing::UPotionEffect_Healing()
{
	Amount = 15.f;
	totalHealed = 0.f;
	healTickRate = 2.5f;
	nextHealTick = healTickRate;
}

void UPotionEffect_Healing::Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp)
{
	Super::Initialize(Instigator, Item, EffectComp);
	Time.TotalTime = Item->GetRarityValue();

	float numOfTicks = Time.TotalTime / healTickRate;
	healAmount = Amount / numOfTicks;
}

void UPotionEffect_Healing::StartTick()
{
	Super::StartTick();
	UE_LOG(LogTemp,Error,TEXT("StartingTick"));
}

void UPotionEffect_Healing::Tick(float DeltaTime)
{
	Time.currTime += DeltaTime;

	if (Time.currTime >= nextHealTick)
	{
		nextHealTick += healTickRate;
		auto StatusComp = AffectedCharacter->GetComponentByClass<UCharacterStatusComponent>();
		if (ensureAlways(StatusComp))
		{
			totalHealed += healAmount;
			UE_LOG(LogTemp, Warning, TEXT("TotalHealed:%f"), totalHealed);
			StatusComp->DamageProcess(-healAmount);
			StatusComp->OnRep_CurrHP();
		}
	}

	if (Time.currTime >= Time.TotalTime)
	{
		OnRep_Time();
		Time.currTime = 0.f;
		GetEffectComponent()->RemoveEffect(this);
	}
	else
	{
		OnRep_Time();
	}
}
