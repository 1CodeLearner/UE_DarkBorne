// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBorneLibrary.h"
#include "../../DBCharacters/DBCharacter.h"
#include "DarkBorne/Status/CharacterStatusComponent.h"
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include "../../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"

float UDarkBorneLibrary::GetDamage(AActor* Instigated)
{
	auto InstigatedStat = Instigated->GetComponentByClass<UCharacterStatusComponent>();
	auto SkillComp = Instigated->GetComponentByClass<UDBRogueSkillComponent>();
	UDBRogueAnimInstance* InstigatedAnimInstance = nullptr;

	{
		if (!(InstigatedStat && SkillComp))
			return 0;

		auto DBCharacter = Cast<ADBCharacter>(Instigated);

		if (DBCharacter)
		{
			auto AnimInstance = DBCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				InstigatedAnimInstance = Cast<UDBRogueAnimInstance>(AnimInstance);
			}
			else return 0;
		}
	}

	float FinalDamage = 0;

	if (ensureAlways(InstigatedAnimInstance->isAttacking))
	{
		FFinalStat InstigatedFinalStat = InstigatedStat->GetFinalStat();

		float WeaponDamage = InstigatedFinalStat.WeaponDamage;
		float Strength = InstigatedFinalStat.Attributes[(int8)EAttributeType::STRENGTH].Range.min;
		float PhysicalDamageBonus = InstigatedFinalStat.PhysDamages[(int8)EPhysicalDamageType::PHYSICALDAMAGEBONUS].Range.min / 100.f;

		if (SkillComp->isSpawnKnife)
		{
			FinalDamage = WeaponDamage + Strength;
		}
		else
		{
			FinalDamage = (WeaponDamage * PhysicalDamageBonus) + WeaponDamage + Strength;
		}

		UE_LOG(LogTemp, Warning, TEXT("WeaponDamage : %f"), WeaponDamage);
		UE_LOG(LogTemp, Warning, TEXT("Strength : %f"), Strength);
		UE_LOG(LogTemp, Warning, TEXT("PhysicalDamageBonus : %f"), PhysicalDamageBonus);
	}

	return FinalDamage;
}

float UDarkBorneLibrary::CalculateDamage(AActor* Instigated, AActor* Received)
{
	auto InstigatedStat = Instigated->GetComponentByClass<UCharacterStatusComponent>();
	auto SkillComp = Instigated->GetComponentByClass<UDBRogueSkillComponent>();
	UDBRogueAnimInstance* InstigatedAnimInstance = nullptr;
	auto ReceivedStat = Received->GetComponentByClass<UCharacterStatusComponent>();

	{
		if (!(InstigatedStat && ReceivedStat && SkillComp))
			return 0;

		auto DBCharacter = Cast<ADBCharacter>(Instigated);

		if (DBCharacter)
		{
			auto AnimInstance = DBCharacter->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				InstigatedAnimInstance = Cast<UDBRogueAnimInstance>(AnimInstance);
			}
			else return 0;
		}
	}

	float FinalDamage = 0;

	if (ensureAlways(InstigatedAnimInstance->isAttacking))
	{
		FFinalStat InstigatedFinalStat = InstigatedStat->GetFinalStat();
		FFinalStat ReceivedFinalStat = ReceivedStat->GetFinalStat();

		float WeaponDamage = InstigatedFinalStat.WeaponDamage;
		float DamageBlockAmount = ReceivedFinalStat.DamageBlockAmt;
		float Strength = InstigatedFinalStat.Attributes[(int8)EAttributeType::STRENGTH].Range.min;
		float PhysicalDamageBonus = InstigatedFinalStat.PhysDamages[(int8)EPhysicalDamageType::PHYSICALDAMAGEBONUS].Range.min / 100.f;

		if (SkillComp->isSpawnKnife)
		{
			FinalDamage = FMath::Max(0.f, (WeaponDamage + Strength) - DamageBlockAmount);
		}
		else
		{
			FinalDamage = (WeaponDamage * PhysicalDamageBonus) + FMath::Max(0.f, (WeaponDamage + Strength) - DamageBlockAmount);
		}

		UE_LOG(LogTemp, Warning, TEXT("WeaponDamage : %f"), WeaponDamage);
		UE_LOG(LogTemp, Warning, TEXT("DamageBlockAmount : %f"), DamageBlockAmount);
		UE_LOG(LogTemp, Warning, TEXT("Strength : %f"), Strength);
		UE_LOG(LogTemp, Warning, TEXT("PhysicalDamageBonus : %f"), PhysicalDamageBonus);
	}


	return FinalDamage;

}

float UDarkBorneLibrary::CalculateInteractionTime(AActor* Instigated)
{
	float Dexterity = 0.f;

	auto StatusComp = Instigated->GetComponentByClass<UCharacterStatusComponent>();
	if (StatusComp)
	{
		Dexterity = StatusComp->GetAddedStat().Attributes[(uint8)EAttributeType::DEXTERITY].Range.min;
		Dexterity += StatusComp->GetBaseStat().Attributes[(uint8)EAttributeType::DEXTERITY].Range.min;
	}

	return Dexterity;
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
		//Character->CharacterStatusComponent->OnRep_CurrHP();
		if (Character->CharacterStatusComponent->CurrHP <= 0.f)
			return true;
	}
	return false;
}
