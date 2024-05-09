// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBorneLibrary.h"
#include "../../DBCharacters/DBCharacter.h"
#include "DarkBorne/Status/CharacterStatusComponent.h"

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
	if (Character && Character->CharacterStatusComponent->CurrHP > 0.f)
	{
		Character->CharacterStatusComponent->CurrHP -= DamageAmount;
		Character->CharacterStatusComponent->OnRep_CurrHP();
		if(Character->CharacterStatusComponent->CurrHP <= 0.f)
			return true;
	}
	return false;
}
