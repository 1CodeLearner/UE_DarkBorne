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
#include "../../Framework/ActorComponents/DBInteractionComponent.h"
#include "../../DBPlayerWidget/DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>

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
	UDBInteractionComponent* InteractionComp = GetOwner()->GetComponentByClass<UDBInteractionComponent>();
	// shift ���������� ����
	if (RogueAnim->isCastingShift) return;

	if (IsUsingItem()) return;

	if(InteractionComp && InteractionComp->IsInteracting())
		return;

	// ���� ������ �����Ǿ������� ���� ������ ��������
	if (RogueSkillComponent->isSpawnKnife)
	{
		// źâ�� ��������� ����
		if (RogueSkillComponent->TKMagazine.IsEmpty()) return;
		RogueThrowKnifeAttack();
	}
	// �ٽ� �⺻�������� / E��ų ���������ʰ� && ���� ������ ������ 
	else if (!RogueSkillComponent->isSpawnKnife && RogueWeaponComponent->hasWeapon)
	{
		ServerRPC_RogueAttack();
	}
	// ������ �ְ� && ������ ���� ���Կ� ���� ������ -> ������ ���
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
			MontageDelegate.BindUFunction(this, "OnMontageEnded");
			AnimInstance->OnMontageEnded.AddUnique(MontageDelegate);
			ActiveItemMontage = RogueWeaponComponent->RogueItems->GetMontage();

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
	UE_LOG(LogTemp, Warning, TEXT("OnMontageEnded invoked by %s"), *GetNameSafe(Montage));

	if(Montage != ActiveItemMontage)
	{
		return;
	}

	UDBRogueWeaponComponent* RogueWeaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	if (RogueWeaponComponent && !ensureAlways(RogueWeaponComponent->RogueItems))
		return;

	auto DBCharacter = Cast<ADBCharacter>(GetOwner());
	if (!DBCharacter)
		return;

	DBCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.Remove(MontageDelegate);
	bUsingItem = false;

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
				EquipmentComp->RemoveItem(RogueWeaponComponent->RogueItems->GetItemObject(), DBCharacter);
			}

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
	UDBEquipmentComponent* RogueEquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
	// ���� ��������� ����
	if(RogueEquipComponent->GetSlots().IsEmpty()) return;
	if (RoguePlayer->RogueWeaponComp->RogueItems == nullptr) return;
	
	if(RogueEquipComponent->GetSlots()[0] && (RogueEquipComponent->GetSlots()[0]->GetItemActor() == RoguePlayer->RogueWeaponComp->RogueItems))
	{
		// ���� ���¸� ���� Ǯ������
		if (RogueSkillComponent->isVanish)
		{
			RogueSkillComponent->DeactiveRogueQSkill();
		}
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

// ���� ��ġ���� ���⼭ ����ϰ� -> ���� -> ��Ƽ ������ �Ѱ��ش�
void UDBRogueAttackComponent::RogueThrowKnifeAttack()
{
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	if(RogueSkillComponent == nullptr) return;
	FHitResult hitInfo;
	// ī�޶� ���� ��ġ
	FVector CameraStartPos = RogueCharacter->camera->GetComponentLocation();
	// Į ���� ��ġ
	if(RogueSkillComponent->TKMagazine[KnifeCount] == nullptr) return;
	//���⼭ ũ���� ����
	FVector KnifeStartPos = RogueSkillComponent->TKMagazine[KnifeCount]->GetActorLocation();
	// endPos : Ʈ���̽� ���� ���� => Į ���� ��ġ + ī�޶��� �պ��� * ����
	FVector endPos = KnifeStartPos + RogueCharacter->camera->GetForwardVector() * 10000;
	FCollisionQueryParams params;
	

	bool isLineHit = GetWorld()->LineTraceSingleByChannel(hitInfo, CameraStartPos, endPos, ECollisionChannel::ECC_Visibility, params);

	// ��Ʈ �� ������, ��Ʈ���� ������ �ʷϻ� 
	//FColor LineColor = isLineHit ? FColor::Red : FColor::Green;
	//DrawDebugLine(GetWorld(), KnifeStartPos, hitInfo.ImpactPoint, LineColor, false, 5.0f, 0, 1.0f);
	//DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 12.f, 24, LineColor, false, 5.0f, 0, 1.0f);

	// Ʈ���̽� �¾Ҵٸ�
	if (isLineHit)
	{	
		// ImpactPoint : Ʈ���̽� �浹 �� ����
		// Direction : �浹 ��ġ - ���� Į�� ��ġ -> �浹 ������ Į ��ġ�� �Ÿ�
		FVector Direction = (hitInfo.ImpactPoint - KnifeStartPos);
		Direction.Normalize();
		// EndRotation : Dirction �� ���Ⱚ
		FRotator EndRotation = Direction.Rotation();
		//EndRotation.Normalize();

		UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *EndRotation.ToString());
		ServerRPC_RogueThrowKnifeAttack(isLineHit, EndRotation, KnifeStartPos);
	}
	else
	{
		FRotator TKRotation = RogueCharacter->camera->GetForwardVector().Rotation();
		TKRotation.Normalize();
	
		UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *TKRotation.ToString());
		ServerRPC_RogueThrowKnifeAttack(isLineHit, TKRotation, KnifeStartPos);
	}
}

void UDBRogueAttackComponent::ServerRPC_RogueThrowKnifeAttack_Implementation(bool isLineHit, FRotator EndRotation, FVector startPos)
{
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	if(RogueSkillComponent == nullptr) return;
	RogueSkillComponent->TKMagazine[KnifeCount]->MultiRPC_RogueThrowKnifeAttack(isLineHit, EndRotation, startPos);
	KnifeCount++;

	if (KnifeCount == RogueSkillComponent->magazineCnt)
	{
		// źâ �ʱ�ȭ
		RogueSkillComponent->TKMagazine.Empty();
		// ������ ī��Ʈ �ʱ�ȭ
		KnifeCount = 0;

		// ��ų ����
		RogueSkillComponent->isSpawnKnife = false;
		RogueSkillComponent->E_CurrCoolTime = 0;

	}
	MultiRPC_RogueThrowKnifeAttack_Update(RogueSkillComponent->isSpawnKnife);
	// ���� ������
	//for (int32 i = 0; i < RogueSkillComponent->magazineCnt; i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("ThrowKnife"));
	//	
	//	RogueSkillComponent->TKMagazine[i]->MultiRPC_RogueThrowKnifeAttack();
	//	
	//}
}
// �����˿� �¾����� ���� ��ų, �����÷��� ������ ���¸� ������Ʈ
void UDBRogueAttackComponent::MultiRPC_RogueThrowKnifeAttack_Update_Implementation(bool isSpawn)
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();
	// ���� ���¸� ���� Ǯ������
	if (RogueSkillComponent->isVanish)
	{
		RogueSkillComponent->DeactiveRogueQSkill();
	}
	if (RoguePlayer->IsLocallyControlled())
	{
		RoguePlayer->PlayerWidget->UpdateEBorder_Active(isSpawn);
	}
}


