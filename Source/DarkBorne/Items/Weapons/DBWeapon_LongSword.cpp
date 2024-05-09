// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_LongSword.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>

ADBWeapon_LongSword::ADBWeapon_LongSword()
{

	CapsuleComp->SetRelativeLocation(FVector(0, 0, -65));
	CapsuleComp->SetCapsuleHalfHeight(55);
	CapsuleComp->SetCapsuleRadius(3);
}
