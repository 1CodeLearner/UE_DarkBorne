// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAnimInstance.h"
#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>

void UDBRogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player 가져오자
	RoguePlayer = Cast<ADBRogueCharacter>(TryGetPawnOwner());
}

void UDBRogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (RoguePlayer)
	{
		// 앞 뒤 (dirV)
		dirV = FVector::DotProduct(RoguePlayer->GetActorForwardVector(), RoguePlayer->GetVelocity());

		// 좌 우 (dirH)
		dirH = FVector::DotProduct(RoguePlayer->GetActorRightVector(), RoguePlayer->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -RoguePlayer->GetBaseAimRotation().Pitch;
		

		pitchAngle = -RoguePlayer->GetBaseAimRotation().GetNormalized().Pitch;

		// GetVelocity의 크기가 0보다 크면 무브 true 로
		if (RoguePlayer->GetVelocity().Length() > 0)
		{
			ShouldMove = true;
		}
		else
		{
			ShouldMove = false;
		}

	}
}

void UDBRogueAnimInstance::AnimNotify_Start_Damage()
{
	UDBRogueWeaponComponent* WeaponComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	
	// 공격중
	isAttacking = true;
	WeaponComp->Dagger->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
}

void UDBRogueAnimInstance::AnimNotify_End_Damage()
{
	UDBRogueWeaponComponent* WeaponComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	// 공격중이 아님
	isAttacking = false;
	WeaponComp->Dagger->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDBRogueAnimInstance::AnimNotify_Start_Hit()
{
	isHitting = true;
}

void UDBRogueAnimInstance::AnimNotify_End_Hit()
{
	isHitting = false;
}
