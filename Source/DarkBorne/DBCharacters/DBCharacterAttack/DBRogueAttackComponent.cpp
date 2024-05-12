// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAttackComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include "../../Items/Weapons/DBWeapon_CloseRange.h"
#include "../DBCharacterSkill/DBRogueSkillComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "../../Framework/Interfaces/ItemInterface.h"
#include "../../Inventory/DBEquipmentComponent.h"
#include "../../Status/DBEffectComponent.h"
#include "../../Items/Consumables/DBConsumable.h"

// Sets default values for this component's properties
UDBRogueAttackComponent::UDBRogueAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	bUsingItem = false;
	bItemActionStarted = false;
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

	if (bItemActionStarted)
		ItemActionUpdate(DeltaTime);
}

void UDBRogueAttackComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_DB_Attack, ETriggerEvent::Triggered, this, &UDBRogueAttackComponent::RogueAttack);
}

void UDBRogueAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UDBRogueAttackComponent, KnifeCount);

	DOREPLIFETIME_CONDITION(UDBRogueAttackComponent, bUsingItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UDBRogueAttackComponent, bItemActionStarted, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UDBRogueAttackComponent, ItemActionTime, COND_OwnerOnly);
}

void UDBRogueAttackComponent::RogueAttack()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	// shift 쓰고있으면 리턴
	if (RogueAnim->isCastingShift) return;

	if (IsUsingItem()) return;

	// 수리검 스킬 수리검 남아있으면 
	if (RogueSkillComponent->isSpawnKnife)
	{
		// 만약 탄창이 비었다면
		if (RogueSkillComponent->TKMagazine.IsEmpty()) return;
		RogueThrowKnifeAttack();
	}
	// 다시 기본공격으로
	// E스킬 쓰고있지않고 && 무기 꺼내고 있지 있으면 
	else if (!RogueSkillComponent->isSpawnKnife && RogueWeaponComponent->hasWeapon)
	{
		ServerRPC_RogueAttack();
	}
	else if (RogueWeaponComponent->RogueItems && RogueWeaponComponent->RogueItems->GetItemObject()->GetSlotType() != ESlotType::WEAPON)
	{
		UseItem();
	}
}

bool UDBRogueAttackComponent::IsUsingItem() const
{
	return bUsingItem;
}

bool UDBRogueAttackComponent::IsInItemAction() const
{
	return bItemActionStarted;
}

void UDBRogueAttackComponent::UseItem()
{
	Server_UseItem();
}

void UDBRogueAttackComponent::Server_UseItem_Implementation()
{
	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	if (!bUsingItem && RogueWeaponComponent->RogueItems && RogueWeaponComponent->RogueItems->GetItemObject()->GetSlotType() != ESlotType::WEAPON)
	{
		ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
		if (RogueWeaponComponent && RogueWeaponComponent->RogueItems && RoguePlayer)
		{
			bUsingItem = true;

			UAnimInstance* AnimInstance = RoguePlayer->GetMesh()->GetAnimInstance();
			Delegate.BindUFunction(this, "OnMontageEnded");
			AnimInstance->OnMontageEnded.AddUnique(Delegate);

			if (RogueWeaponComponent->RogueItems->Implements<UItemInterface>())
			{
				ItemActionTime.TotalTime = RogueWeaponComponent->RogueItems->GetItemObject()->GetRarityValue();

				bItemActionStarted = true;
				OnRep_bItemActionStarted();
			}

			Multicast_StartMontage();
		}
	}
}

void UDBRogueAttackComponent::Multicast_StartMontage_Implementation()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	if (RoguePlayer && RogueWeaponComponent && RogueWeaponComponent->RogueItems)
	{
		RogueWeaponComponent->RogueItems->PlayMontage(RoguePlayer, "");
	}
}

void UDBRogueAttackComponent::Multicast_StopMontage_Implementation()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	if (RoguePlayer)
	{
		RoguePlayer->GetMesh()->GetAnimInstance()->Montage_JumpToSection("StopMontage");
	}
}

void UDBRogueAttackComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("OnMontageEnded in consumable invoked"));
	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	if (RogueWeaponComponent && !ensureAlways(RogueWeaponComponent->RogueItems))
		return;

	auto DBCharacter = Cast<ADBCharacter>(GetOwner());
	if (!DBCharacter)
		return;

	DBCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.Remove(Delegate);

	if (!bInterrupted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Interrupted"));

		if (!bItemActionStarted)
		{
			auto EffectComp = DBCharacter->GetComponentByClass<UDBEffectComponent>();
			if (ensureAlways(EffectComp))
			{
				auto ConsumableItem = Cast<ADBConsumable>(RogueWeaponComponent->RogueItems);
				if (ConsumableItem)
				{
					EffectComp->AddEffect(DBCharacter, ConsumableItem);
				}
			}

			auto EquipmentComp = DBCharacter->GetComponentByClass<UDBEquipmentComponent>();
			if (EquipmentComp)
			{
				EquipmentComp->RemoveItem(RogueWeaponComponent->RogueItems->GetItemObject(), EquipmentComp);
			}

			RogueWeaponComponent->RemoveRogueItems();

			RogueWeaponComponent->AttachWeapon();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item action on %s was stopped by player"), *GetNameSafe(RogueWeaponComponent->RogueItems));
			bItemActionStarted = false;
			OnRep_bItemActionStarted();
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Item Montage on %s Interrupted"), *GetNameSafe(RogueWeaponComponent->RogueItems));

	bUsingItem = false;
}

void UDBRogueAttackComponent::OnRep_bItemActionStarted()
{
	auto Character = Cast<ACharacter>(GetOwner());
	if (Character && Character->IsLocallyControlled())
	{
		bItemActionStarted ? OnBeginItemAction.ExecuteIfBound() : OnEndItemAction.ExecuteIfBound();
	}
}

void UDBRogueAttackComponent::ItemActionUpdate(float DeltaTime)
{
	ItemActionTime.CurrTime += DeltaTime;
	OnRep_ItemActionTime();
	if (ItemActionTime.CurrTime >= ItemActionTime.TotalTime)
	{
		bItemActionStarted = false;
		OnRep_bItemActionStarted();

		ItemActionTime.CurrTime = 0.f;
		Multicast_StopMontage();
	}
}

void UDBRogueAttackComponent::OnRep_ItemActionTime()
{
	OnItemActionUpdate.ExecuteIfBound(ItemActionTime.CurrTime, ItemActionTime.TotalTime);
}

void UDBRogueAttackComponent::StopItemAction()
{
	if (ensureAlways(bUsingItem) && ensureAlways(bItemActionStarted))
		Server_StopItemAction();
}

void UDBRogueAttackComponent::Server_StopItemAction_Implementation()
{
	if (bUsingItem)
	{
		ItemActionTime.CurrTime = 0.f;
		Multicast_StopMontage();
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

	if (RoguePlayer->RogueWeaponComp->RogueItems == nullptr) return;
	if (RoguePlayer->RogueWeaponComp->EquipSlotArray[0] && RoguePlayer->RogueWeaponComp->EquipSlotArray[0]->GetItemActor() == RoguePlayer->RogueWeaponComp->RogueItems)
	{
		// 은신 상태면 은신 풀어주자
		if (RogueSkillComponent->isVanish)
		{
			RogueSkillComponent->DeactiveRogueQSkill();
		}
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
	ServerRPC_RogueThrowKnifeAttack();
}

void UDBRogueAttackComponent::ServerRPC_RogueThrowKnifeAttack_Implementation()
{
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();

	RogueSkillComponent->TKMagazine[KnifeCount]->MultiRPC_RogueThrowKnifeAttack();
	KnifeCount++;


	if (KnifeCount == RogueSkillComponent->magazineCnt)
	{
		// 탄창 초기화
		RogueSkillComponent->TKMagazine.Empty();
		// 나이프 카운트 초기화
		KnifeCount = 0;

		// 스킬 끄기
		RogueSkillComponent->isSpawnKnife = false;
		RogueSkillComponent->E_CurrCoolTime = 0;

	}
	MultiRPC_RogueThrowKnifeAttack();
	// 전부 던지기
	//for (int32 i = 0; i < RogueSkillComponent->magazineCnt; i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ThrowKnife"));
	//	
	//	RogueSkillComponent->TKMagazine[i]->MultiRPC_RogueThrowKnifeAttack();
	//	
	//}
}

void UDBRogueAttackComponent::MultiRPC_RogueThrowKnifeAttack_Implementation()
{
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	// 은신 상태면 은신 풀어주자
	if (RogueSkillComponent->isVanish)
	{
		RogueSkillComponent->DeactiveRogueQSkill();
	}
}


