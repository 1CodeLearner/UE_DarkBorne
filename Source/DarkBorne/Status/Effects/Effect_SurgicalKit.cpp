// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect_SurgicalKit.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../CharacterStatusComponent.h"
#include "../DBEffectComponent.h"

void UEffect_SurgicalKit::StartTick()
{
	Super::StartTick();

	auto StatusComp = AffectedCharacter->GetComponentByClass<UCharacterStatusComponent>();
	if(StatusComp->CurrHP > 0.f)
	{
		StatusComp->DamageProcess(-StatusComp->MaxHP);
		StatusComp->OnRep_CurrHP();
	}

	GetEffectComponent()->RemoveEffect(this);
}
