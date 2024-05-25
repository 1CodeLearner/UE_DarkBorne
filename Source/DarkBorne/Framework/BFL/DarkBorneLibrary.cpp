// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBorneLibrary.h"
#include "../../DBCharacters/DBCharacter.h"
#include "DarkBorne/Status/CharacterStatusComponent.h"
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include "../../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"

float UDarkBorneLibrary::CalculateDamage(AActor* Instigated, AActor* Received)
{
	auto InstigatedStat = Instigated->GetComponentByClass<UCharacterStatusComponent>();
	auto SkillComp = Instigated->GetComponentByClass<UDBRogueSkillComponent>();
	UDBRogueAnimInstance* InstigatedAnimInstance = nullptr;
	auto ReceivedStat = Received->GetComponentByClass<UCharacterStatusComponent>();

	{
		if (!(InstigatedStat && ReceivedStat && SkillComp))
			return -1;

		auto DBCharacter = Cast<ADBCharacter>(Instigated);

		if (DBCharacter)
		{
			auto AnimInstance = DBCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				InstigatedAnimInstance = Cast<UDBRogueAnimInstance>(AnimInstance);
			}
			else return -1;
		}
	}



	if (InstigatedAnimInstance->isAttacking)
	{	
		if (InstigatedAnimInstance->isCastingShift)
		{

		}
		else if (SkillComp->isSpawnKnife)
		{

		}
		else
		{

		}
	}




	return -1.f;

}

bool UDarkBorneLibrary::ApplyDamage(AActor* Received, AActor* Instigated)
{
	return false;
}

bool UDarkBorneLibrary::ApplyDamageAmount(AActor* Received, float DamageAmount)
{
	auto Character = Cast<ADBCharacter>(Received);
	if (Character && Character->CharacterStatusComponent->CurrHP > 0.f)
	{
		Character->CharacterStatusComponent->DamageProcess(DamageAmount);
		Character->CharacterStatusComponent->OnRep_CurrHP();
		if (Character->CharacterStatusComponent->CurrHP <= 0.f)
			return true;
	}
	return false;
}
