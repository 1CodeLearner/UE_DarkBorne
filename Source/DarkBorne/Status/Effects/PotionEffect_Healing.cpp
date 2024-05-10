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
	TotalTime = Item->GetRarityValue();

	float numOfTicks = TotalTime / healTickRate;
	healAmount = Amount / numOfTicks;
}

void UPotionEffect_Healing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DeltaTime;
	if (currTime >= nextHealTick)
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

	if (currTime >= TotalTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("TotalHealed:%f, Amount:%f"), totalHealed, Amount);
		//if (totalHealed < Amount) {
		//	auto StatusComp = AffectedCharacter->GetComponentByClass<UCharacterStatusComponent>();
		//	if (ensureAlways(StatusComp))
		//	{
		//		totalHealed += healAmount;
		//		UE_LOG(LogTemp, Warning, TEXT("TotalHealed:%f"), totalHealed);
		//		StatusComp->DamageProcess(-healAmount);
		//	}
		//}
		OnEveryTick.ExecuteIfBound(TotalTime, currTime);
		GetEffectComponent()->RemoveEffect(this);
	}
	else
	{
		OnEveryTick.ExecuteIfBound(TotalTime, currTime);
	}
}
