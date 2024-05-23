// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAnimInstance.h"
#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include "Net/UnrealNetwork.h"
#include "../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"
#include "../DBCharacters/DBCharacterAttack/DBRogueAttackComponent.h"
#include "../Inventory/DBEquipmentComponent.h"
void UDBRogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UDBRogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(TryGetPawnOwner());
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

		jumpVelocity = RoguePlayer->GetVelocity().Z;

		// 높이가 0보다 크면 falling
		if (jumpVelocity > 0)
		{
			isFalling = true;
		}
		else if(jumpVelocity == 0)
		{
			isFalling = false;
		}
	}
}

void UDBRogueAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBRogueAnimInstance, isDeath);
	DOREPLIFETIME(UDBRogueAnimInstance, isDoubleJumping);
	DOREPLIFETIME(UDBRogueAnimInstance, isCastingShift);
}

void UDBRogueAnimInstance::AnimNotify_Start_Damage()
{
	UDBRogueWeaponComponent* WeaponComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	UDBRogueSkillComponent* SkillComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	UDBRogueAttackComponent* AttackComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueAttackComponent>();
	UDBEquipmentComponent* RogueEquipComponent = TryGetPawnOwner()->GetComponentByClass<UDBEquipmentComponent>();
	// 공격중
	// 무기슬롯에 아이템이 있고 아이템 꺼내고 있다면
	if (RogueEquipComponent->GetSlots()[0] && WeaponComp->hasWeapon)
	{
		isAttacking = true;
		//무기의 콜리전 켜주기
		if(WeaponComp->RogueItems == nullptr) return;
		WeaponComp->RogueItems->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}
	if(SkillComp->isSpawnKnife)//!SkillComp->TKMagazine.IsEmpty())
	{
		isAttacking = true;
	}
	
}

void UDBRogueAnimInstance::AnimNotify_End_Damage()
{
	UDBRogueWeaponComponent* WeaponComp = TryGetPawnOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	ADBWeapon_CloseRange* WeaponComponentDagger = Cast<ADBWeapon_CloseRange>(WeaponComp->Dagger);
	UDBEquipmentComponent* RogueEquipComponent = TryGetPawnOwner()->GetComponentByClass<UDBEquipmentComponent>();
	// 공격중이 아님
	if (RogueEquipComponent->GetSlots()[0] && WeaponComp->hasWeapon)
	{
	isAttacking = false;
	WeaponComp->RogueItems->CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UDBRogueAnimInstance::AnimNotify_Start_Hit()
{
	isHitting = true;
}

void UDBRogueAnimInstance::AnimNotify_End_Hit()
{
	isHitting = false;
}

void UDBRogueAnimInstance::AnimNotify_Jump_End()
{
	isFalling = false;
}

void UDBRogueAnimInstance::AnimNotify_DoubleJumpStart()
{
	isDoubleJumping = true;
}

void UDBRogueAnimInstance::AnimNotify_DoubleJumpEnd()
{
	isDoubleJumping = false;
}

void UDBRogueAnimInstance::AnimNotify_Start_Shift()
{
	isCastingShift = true;
}

void UDBRogueAnimInstance::AnimNotify_End_Shift()
{
	isCastingShift = false;
}
