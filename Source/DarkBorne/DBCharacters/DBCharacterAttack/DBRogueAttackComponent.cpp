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

	SetIsReplicatedByDefault(true);
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
	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	// ������ ��ų ������ ���������� 
	if (RogueSkillComponent->isSpawnKnife)
	{	
		// ���� źâ�� ����ٸ�
		if(RogueSkillComponent->TKMagazine.IsEmpty())
		{
			
			return;
		}
		RogueThrowKnifeAttack();
	}
	// �ٽ� �⺻��������
	// E��ų ���������ʰ� && ���� ������ ���� ������ 
	else if (!RogueSkillComponent->isSpawnKnife && RogueWeaponComponent->hasWeapon)
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
	// �ܰ��� ��� ������ 

	//���� ������ : ���� ����(EquipSlotArray[0])�� ������ ������ ����ְ�
	//���⸦ ������(��������) ���� ���¿��� �Ʒ� �Լ��� �����Ͽ� ������ ����
	if (RoguePlayer->RogueWeaponComp->EquipSlotArray[0])
	{
		
		RogueSkillComponent->CurrVanishTime = 0;
		RogueSkillComponent->DeactiveRogueQSkill();
		if (comboCnt == 0)
		{
			comboCnt++;
			comboCurrTime = 0;

			// �ܰ� �����ۿ� �ִ� �ִԸ�Ÿ�� ����
			RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack1"));
		}
		else if (comboCnt == 1)
		{
			// �޺��ּҽð� <= ����ð� �̰� ����ð� <= �ִ�ð�
			if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
			{
				comboCnt++;
				comboCurrTime = 0;

				// �ܰ� �����ۿ� �ִ� �ִԸ�Ÿ�� ����
				RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack2"));
			}
		}
		else if (comboCnt == 2)
		{
			// �޺��ּҽð� <= ����ð� �̰� ����ð� <= �ִ�ð�
			if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
			{
				comboCnt++;
				comboCurrTime = 0;

				// �ܰ� �����ۿ� �ִ� �ִԸ�Ÿ�� ����
				RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack3"));
			}
		}
		//else if (comboCnt == 3)
		//{
		//	// �޺��ּҽð� <= ����ð� �̰� ����ð� <= �ִ�ð�
		//	if (comboMinTime <= comboCurrTime && comboCurrTime <= comboMaxTime)
		//	{
		//		comboCnt++;
		//		comboCurrTime = 0;
		//
		//		// �ܰ� �����ۿ� �ִ� �ִԸ�Ÿ�� ����
		//		RoguePlayer->RogueWeaponComp->RogueItems->PlayMontage(RoguePlayer, FName("Attack4"));
		//	}
		//}
	}
}

void UDBRogueAttackComponent::UpdateComboCount(float DeltaTime)
{
	// �޺� ī��Ʈ�� 0���� ũ�� (�޺� ����)
	if (comboCnt > 0)
	{
		//�޺� ����ð� = �޺� ����ð� + ��ŸŸ�� -> �޺� ����ð��� �������Ѷ�
		comboCurrTime += DeltaTime;
		//�޺� ���� �ð��� �ִ�ð����� Ŀ���� ��
		if (comboCurrTime > comboMaxTime)
		{
			// �޺� ī��Ʈ�� �ʱ�ȭ ���Ѷ� -> �޺� ó������ �����ϱ� ���ؼ�
			comboCnt = 0;

		}
	}
}

void UDBRogueAttackComponent::RogueThrowKnifeAttack()
{	
	ServerRPC_RogueThrowKnifeAttack();
}


void UDBRogueAttackComponent::ServerRPC_RogueThrowKnifeAttack_Implementation()
{
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	// ������ ī��Ʈ�� źâ ������ �������ٸ� 
	if(KnifeCount == RogueSkillComponent->magazineCnt)
	{
		// ��ų ����
		RogueSkillComponent->isSpawnKnife = false;
		return;
	}

	RogueSkillComponent->TKMagazine[KnifeCount]->MultiRPC_RogueThrowKnifeAttack();
	KnifeCount++;
	//RogueSkillComponent->TKMagazine.RemoveAt(DeleteKnifeCount);
	//DeleteKnifeCount++;
	UE_LOG(LogTemp, Warning, TEXT("KnifeCount is : %d"), KnifeCount);
	// �Ѱ��� ������
	// ��Ŭ���� �Ͽ��ٸ�
	// tkmagazine n���� MultiRPC_RogueThrowKnifeAttack ����
	// �׸��� n++



	
	// ���� ������
	//for (int32 i = 0; i < RogueSkillComponent->magazineCnt; i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ThrowKnife"));
	//	
	//	RogueSkillComponent->TKMagazine[i]->MultiRPC_RogueThrowKnifeAttack();
	//	
	//}
}



