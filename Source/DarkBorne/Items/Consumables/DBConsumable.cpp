// Fill out your copyright notice in the Description page of Project Settings.


#include "DBConsumable.h"
#include "../../DBCharacters/DBCharacter.h"
#include "Animation/AnimInstance.h"
#include "../../Status/DBEffectComponent.h"
#include "../../Inventory/DBEquipmentComponent.h"
#include "../../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"

TSubclassOf<UDBEffect> ADBConsumable::GetEffectClass() const
{
	return EffectClass;
}