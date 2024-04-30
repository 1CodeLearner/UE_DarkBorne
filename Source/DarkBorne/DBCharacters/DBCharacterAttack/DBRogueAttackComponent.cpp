// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAttackComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include "../../Items/Weapons/DBWeapon_CloseRange.h"
#include "../DBCharacterSkill/DBRogueSkillComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>


// Sets default values for this component's properties
UDBRogueAttackComponent::UDBRogueAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UDBRogueAttackComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UDBRogueAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateComboCount(DeltaTime);
}

void UDBRogueAttackComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_DB_Attack, ETriggerEvent::Triggered, this, &UDBRogueAttackComponent::RogueAttack);
	//enhancedInputComponent->BindAction(ia_DB_Attack, ETriggerEvent::Triggered, this, &UDBRogueAttackComponent::RogueAttack);

}

void UDBRogueAttackComponent::RogueAttack()
{
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();

	// 수리검 스킬 수리검 남아있으면 
	if (RogueSkillComponent->isSpawnKnife)
	{
		if(RogueSkillComponent->TKMagazine.IsEmpty()) return;
		RogueThrowKnifeAttack();
	}
	// 다시 기본공격으로
	else if (!RogueSkillComponent->isSpawnKnife)
	{

		ServerRPC_RogueAttack();
	}
	
}

void UDBRogueAttackComponent::ServerRPC_RogueAttack_Implementation()
{
	MultiRPC_RogueAttack();
}

void UDBRogueAttackComponent::MultiRPC_RogueAttack_Implementation()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	
	if (RoguePlayer->RogueWeaponComp->EquipSlotArray.IsEmpty()) return;
	// 단검을 들고 있으면 
	if (RoguePlayer->RogueWeaponComp->EquipSlotArray[0] != nullptr)
	{
		RogueSkillComponent->CurrVanishTime = 0;
		RogueSkillComponent->DeactiveRogueQSkill();
		if (comboCnt == 0)
		{
			comboCnt++;
			comboCurrTime = 0;

			// 단검 아이템에 있는 애님몽타주 실행
			RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack1"));
		}
		else if (comboCnt == 1)
		{
			// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
			if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
			{
				comboCnt++;
				comboCurrTime = 0;

				// 단검 아이템에 있는 애님몽타주 실행
				RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack2"));
			}
		}
		else if (comboCnt == 2)
		{
			// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
			if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
			{
				comboCnt++;
				comboCurrTime = 0;

				// 단검 아이템에 있는 애님몽타주 실행
				RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack3"));
			}
		}
		//else if (comboCnt == 3)
		//{
		//	// 콤보최소시간 <= 현재시간 이고 현재시간 <= 최대시간
		//	if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
		//	{
		//		comboCnt++;
		//		comboCurrTime = 0;
		//
		//		// 단검 아이템에 있는 애님몽타주 실행
		//		RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack4"));
		//	}
		//}
	}
}

void UDBRogueAttackComponent::UpdateComboCount(float DeltaTime)
{
	// 콤보 카운트가 0보다 크면 (콤보 시작)
	if (comboCnt > 0)
	{
		//콤보 현재시간 = 콤보 현재시간 + 델타타임 -> 콤보 현재시간을 증가시켜라
		comboCurrTime += DeltaTime;
		//콤보 현재 시간이 최대시간보다 커졌을 때
		if (comboCurrTime > comboMaxTime)
		{
			// 콤보 카운트를 초기화 시켜라 -> 콤보 처음부터 시작하기 위해서
			comboCnt = 0;

		}
	}
}

void UDBRogueAttackComponent::RogueThrowKnifeAttack()
{	
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	//누르면 TKmagazine에 있는 탄창을 0부터 ~ num()까지 RemoveAt


	UE_LOG(LogTemp, Warning, TEXT("ThrowKnife"));
	RogueSkillComponent->ThrowingKnife->isThrowing = true;
	RogueSkillComponent->ThrowingKnife->projectileComponent->ProjectileGravityScale = 1.0f;
	RogueSkillComponent->ThrowingKnife->projectileComponent->SetActive(true, true);
 	RogueSkillComponent->ThrowingKnife->projectileComponent->SetVelocityInLocalSpace(FVector(2000, 0, 0));
}

