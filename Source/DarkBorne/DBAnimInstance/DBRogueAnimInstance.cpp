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
		// �� �� (dirV)
		dirV = FVector::DotProduct(RoguePlayer->GetActorForwardVector(), RoguePlayer->GetVelocity());

		// �� �� (dirH)
		dirH = FVector::DotProduct(RoguePlayer->GetActorRightVector(), RoguePlayer->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -RoguePlayer->GetBaseAimRotation().Pitch;
		
		pitchAngle = -RoguePlayer->GetBaseAimRotation().GetNormalized().Pitch;

		// GetVelocity�� ũ�Ⱑ 0���� ũ�� ���� true ��
		if (RoguePlayer->GetVelocity().Length() > 0)
		{
			ShouldMove = true;
		}
		else
		{
			ShouldMove = false;
		}

		jumpVelocity = RoguePlayer->GetVelocity().Z;

		// ���̰� 0���� ũ�� falling
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
	// ������
	// ���⽽�Կ� �������� �ְ� ������ ������ �ִٸ�
	if (RogueEquipComponent->GetSlots()[0] && WeaponComp->hasWeapon)
	{
		isAttacking = true;
		//������ �ݸ��� ���ֱ�
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
	// �������� �ƴ�
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
