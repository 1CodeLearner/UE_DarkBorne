// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable_SurgicalKit.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"

AConsumable_SurgicalKit::AConsumable_SurgicalKit()
{
	currTime = 0.f;
	totalTime = 0.f;
	bMontageStart = false;
	bTimeElapsed = false;
}

bool AConsumable_SurgicalKit::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	if (Super::PlayMontage(PlayerCharacter, SectionName))
	{
		if (HasAuthority())
		{
			totalTime = ItemObj->GetRarityValue();
			bMontageStart = true;
		}
		return true;
	}
	return false;
}

void AConsumable_SurgicalKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		OnRep_Owner();
	}
	if (bMontageStart)
	{
		currTime += DeltaTime;
		OnRep_currTime();

		if (currTime >= totalTime)
		{
			bMontageStart = false;
			bTimeElapsed = true;
			currTime = 0.f;

			OnRep_bTimeElapsed();
		}
	}
}

void AConsumable_SurgicalKit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(AConsumable_SurgicalKit, currTime, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AConsumable_SurgicalKit, totalTime, COND_OwnerOnly);

	DOREPLIFETIME(AConsumable_SurgicalKit, currTime);
	DOREPLIFETIME(AConsumable_SurgicalKit, totalTime);
	DOREPLIFETIME(AConsumable_SurgicalKit, bTimeElapsed);
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

void AConsumable_SurgicalKit::OnRep_currTime()
{
	OnInteractTimeUpdate.ExecuteIfBound(currTime, totalTime);
}

void AConsumable_SurgicalKit::OnRep_bTimeElapsed()
{
	OnInteractFinished.ExecuteIfBound();
}