// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_Dagger.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>

ADBWeapon_Dagger::ADBWeapon_Dagger()
{
	CapsuleComp->SetRelativeLocation(FVector(0, 0, 25));
	CapsuleComp->SetCapsuleHalfHeight(16);
	CapsuleComp->SetCapsuleRadius(3);
}