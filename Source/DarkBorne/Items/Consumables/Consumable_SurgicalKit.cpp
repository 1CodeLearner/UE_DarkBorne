// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable_SurgicalKit.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"

void AConsumable_SurgicalKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		OnRep_Owner();
	}
}

void AConsumable_SurgicalKit::OnRep_Owner()
{
	if (Owner)
	{
		SMComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	}
	else {
		SMComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	}
}