// Fill out your copyright notice in the Description page of Project Settings.


#include "J_TestCharacter.h"

void AJ_TestCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (ensureAlways(DT_CharacterStats))
		CharacterBaseStat = *DT_CharacterStats->FindRow<FCharacterBaseStat>
		(
			RowName,
			FString::Printf(TEXT("Getting CharacterBaseStat"))
		);

	if (ensure(CharacterBaseStat.Attributes.Num() == FinalStat.Attributes.Num()))
	{
		for (int32 i = 0; i < CharacterBaseStat.Attributes.Num(); ++i)
			FinalStat.Attributes[i] += CharacterBaseStat.Attributes[i];
	}
}

const FFinalStat& AJ_TestCharacter::GetFinalStat() const
{
	return FinalStat;
}
