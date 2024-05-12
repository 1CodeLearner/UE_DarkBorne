// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon.h"
#include "../../Inventory/ItemObject.h"

ADBWeapon::ADBWeapon()
{
	SMComp->SetCollisionProfileName(TEXT("WeaponSMColl"));
}

void ADBWeapon::Initialize(UItemObject* ItemObject)
{
	Super::Initialize(ItemObject);

	if (ItemObj)
	{
		WeaponDamage = ItemObj->GetRarityValue();
	}
}
