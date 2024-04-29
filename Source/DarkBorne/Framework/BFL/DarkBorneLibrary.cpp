// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBorneLibrary.h"
#include "../../DBCharacters/DBCharacter.h"

float UDarkBorneLibrary::CalculateDamage(AActor* Instigated, const TArray<FItem>& Items)
{
	return 0.0f;
}

bool UDarkBorneLibrary::ApplyDamage(AActor* Received, AActor* Instigated)
{
	return false;
}

bool UDarkBorneLibrary::ApplyDamageAmount(AActor* Received, float DamageAmount)
{
	auto Character = Cast<ADBCharacter>(Received);
	if (Character && Character->CurrHP > 0.f)
	{
		Character->CurrHP -= DamageAmount;
		Character->OnRep_CurrHP();
		if(Character->CurrHP <= 0.f)
			return true;
	}
	return false;
}
