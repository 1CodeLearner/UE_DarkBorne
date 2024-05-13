// Fill out your copyright notice in the Description page of Project Settings.


#include "J_TestCharacter.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/InventoryMainWidget.h"


AJ_TestCharacter::AJ_TestCharacter()
{
	PlayerEquipmentComp = CreateDefaultSubobject<UPlayerEquipmentComponent>("PlayerEquipmentComp");

}

void AJ_TestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		if (ensureAlways(DT_CharacterStats))
			CharacterBaseStat = *DT_CharacterStats->FindRow<FCharacterBaseStat>
			(
				RowName,
				FString::Printf(TEXT("Getting CharacterBaseStat"))
			);
	}
	
	if (!HasAuthority() && IsLocallyControlled()) 
	{
		InvMainWidget = CreateWidget<UInventoryMainWidget>(GetController<APlayerController>(), InvMainWidgetClass);
		
	}
}

const FAddedStat& AJ_TestCharacter::GetFinalStat() const
{
	return AddedStat;
}
