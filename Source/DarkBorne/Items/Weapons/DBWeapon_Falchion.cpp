// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_Falchion.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>

ADBWeapon_Falchion::ADBWeapon_Falchion()
{

	CapsuleComp->SetRelativeLocation(FVector(0, 0, 40));
	CapsuleComp->SetCapsuleHalfHeight(40);
	CapsuleComp->SetCapsuleRadius(8);
}
