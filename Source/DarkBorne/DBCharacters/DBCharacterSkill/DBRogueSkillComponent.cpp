// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "../../Items/Weapons/DBWeapon_CloseRange.h"
#include "Net/UnrealNetwork.h"
#include "../../Items/Weapons/RogueThrowingKnife.h"
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ArrowComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Animation/AnimMontage.h>
#include "../../DBPlayerWidget/DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "../DBCharacterAttack/DBRogueAttackComponent.h"
#include "../../Inventory/DBEquipmentComponent.h"
#include "../../Inventory/InventoryMainWidget.h"

// Sets default values for this component's properties
UDBRogueSkillComponent::UDBRogueSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UDBRogueSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	Q_CurrCoolTime = Q_MaxCoolTime;
	E_CurrCoolTime = E_MaxCoolTime;

	OnRep_CurrQSkill();
	OnRep_CurrESkill();

	MyCharacter = Cast<ADBCharacter>(GetOwner());
}


// Called every frame
void UDBRogueSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateRogueQSkill(DeltaTime);
	UpdateRogueESkill(DeltaTime);
}

void UDBRogueSkillComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_Q_Skill, ETriggerEvent::Triggered, this, &UDBRogueSkillComponent::ActiveRogueQSkill);
	enhancedInputComponent->BindAction(ia_E_Skill, ETriggerEvent::Triggered, this, &UDBRogueSkillComponent::ActiveRogueESkill);
	enhancedInputComponent->BindAction(ia_Shift_Skill, ETriggerEvent::Triggered, this, &UDBRogueSkillComponent::ActiveRogueShiftSkill);
}

void UDBRogueSkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBRogueSkillComponent, isVanish);
	DOREPLIFETIME(UDBRogueSkillComponent, MaxVanishTime);
	DOREPLIFETIME(UDBRogueSkillComponent, CurrVanishTime);

	DOREPLIFETIME(UDBRogueSkillComponent, TKMagazine);
	DOREPLIFETIME(UDBRogueSkillComponent, isSpawnKnife);
	DOREPLIFETIME(UDBRogueSkillComponent, magazineCnt);

	DOREPLIFETIME(UDBRogueSkillComponent, Q_MaxCoolTime);
	DOREPLIFETIME(UDBRogueSkillComponent, Q_CurrCoolTime);

	DOREPLIFETIME(UDBRogueSkillComponent, E_MaxCoolTime);
	DOREPLIFETIME(UDBRogueSkillComponent, E_CurrCoolTime);

}

void UDBRogueSkillComponent::OnRep_CurrQSkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());

	// 플레이어 위젯이 없으면 리턴
	if (RoguePlayer->PlayerWidget == nullptr) return;
	RoguePlayer->PlayerWidget->UpdateQSkillBar(Q_CurrCoolTime, Q_MaxCoolTime);
	RoguePlayer->PlayerWidget->UpdateQBorder(Q_CurrCoolTime, Q_MaxCoolTime);
}

void UDBRogueSkillComponent::OnRep_CurrESkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());

	// 플레이어 위젯이 없으면 리턴
	if (RoguePlayer->PlayerWidget == nullptr) return;
	RoguePlayer->PlayerWidget->UpdateESkillBar(E_CurrCoolTime, E_MaxCoolTime);
	RoguePlayer->PlayerWidget->UpdateEBorder(E_CurrCoolTime, E_MaxCoolTime);
}

void UDBRogueSkillComponent::UpdateRogueQSkill(float DeltaTime)
{	//은신 상태가 되었다면
	if (isVanish)
	{
		UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
		ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
		UDBEquipmentComponent* RogueEquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
		// 내 입장에서는 스킬 위젯만 업데이트 시킨다.
		if (RoguePlayer->IsLocallyControlled())
		{
			RoguePlayer->PlayerWidget->UpdateQBorder_Active(isVanish);
		}

		// 상대 입장에서만 무기를 투명화 시킨다.
		if (!RoguePlayer->IsLocallyControlled())
		{
			// 무기 가지고 있으면
			if (RogueEquipComponent->GetSlots()[0] && weaponComponent->RogueItems)
			{
				for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
				{
					// 무기도 은신 머티리얼로 설정
					weaponComponent->RogueItems->SMComp->SetMaterial(i, VanishMat);
				}
			}
		}
		// 현재 은신시간이 최대시간보다 작다면
		if (CurrVanishTime < MaxVanishTime)
		{
			// 현재 은신 시간에 델타타임 더하기
			CurrVanishTime += DeltaTime;

			//현재 은신시간이 최대 은신시간과 같아졌다면
			if (CurrVanishTime >= MaxVanishTime)
			{
				// 은신 비활성화
				DeactiveRogueQSkill();
			}
		}
	}
	// 은신 false고 현재 쿨타임이 최대 쿨보다 작다면
	else if (!isVanish && Q_CurrCoolTime < Q_MaxCoolTime)
	{
		// curr cooltime에 델타타임 더한다
		Q_CurrCoolTime += DeltaTime;

		//서버 플레이어를 위한 호출
		OnRep_CurrQSkill();

		ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());

		if (RoguePlayer->IsLocallyControlled())
		{
			RoguePlayer->PlayerWidget->UpdateQBorder_Active(isVanish);
		}
	}
}

void UDBRogueSkillComponent::ActiveRogueQSkill()
{
	// curr cooltime >= max time이 되었다면
	if (Q_CurrCoolTime >= Q_MaxCoolTime)
	{
		if (MyCharacter && MyCharacter->IsInventoryInViewport())
			return;

		// q skill 활성화
		//서버 함수 실행
		ServerRPC_ActiveRogueQSkill();
	}
}

void UDBRogueSkillComponent::ServerRPC_ActiveRogueQSkill_Implementation()
{
	// 서버->클라이언트 전달.
	MultiRPC_ActiveRogueQSkill();
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueQSkill_Implementation()
{
	UDBRogueAttackComponent* AttackComponent = GetOwner()->GetComponentByClass<UDBRogueAttackComponent>();
	if (AttackComponent && AttackComponent->IsInItemAction())
	{
		return;
	}

	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	UDBEquipmentComponent* RogueEquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();

	if (RogueAnim->isCastingShift) return;
	if (isVanish) return;
	isVanish = true;
	// 은신을 사용 했다면
	if (isVanish)
	{
		// 나의 것이라면
		if (RoguePlayer->IsLocallyControlled())
		{
			// 화면 회색 처리 및 카메라 효과, 사운드
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = true;
			RoguePlayer->camera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);

			RoguePlayer->camera->PostProcessSettings.bOverride_VignetteIntensity = true;
			RoguePlayer->camera->PostProcessSettings.VignetteIntensity = 0.7f;

			RoguePlayer->camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
			RoguePlayer->camera->PostProcessSettings.SceneFringeIntensity = 5.0f;

			RoguePlayer->camera->PostProcessSettings.bOverride_ChromaticAberrationStartOffset = true;
			RoguePlayer->camera->PostProcessSettings.ChromaticAberrationStartOffset = 0;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), VanishStartSFX, RoguePlayer->GetActorLocation());
		}
		//!IsLocallyControlled()
		else
		{
			// 로그 캐릭터 머티리얼 인덱스 0 부터 last까지 가져와서
			for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
			{
				// 기존 캐릭터 메쉬 머티리얼을 투명한 머티리얼로 바꾸자
				RoguePlayer->GetMesh()->SetMaterial(i, VanishMat);
			}
			// 무기 가지고 있으면
			if (RogueEquipComponent->GetSlots()[0] && weaponComponent->RogueItems)
			{
				// 무기 머티리얼 인덱스 가져와서
				for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
				{
					// 무기를 은신 머티리얼로 설정
					weaponComponent->RogueItems->SMComp->SetMaterial(i, VanishMat);
				}

			}
		}
	}
}

void UDBRogueSkillComponent::DeactiveRogueQSkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	UDBEquipmentComponent* RogueEquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();

	isVanish = false;

	// 최대 지속시간에 도달했다면
	if (!isVanish)
	{
		//은신 시간, 은신 쿨타임 초기화
		CurrVanishTime = 0;
		Q_CurrCoolTime = 0;

		if (RoguePlayer->IsLocallyControlled())
		{
			//화면 원래대로
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = false;
			RoguePlayer->camera->PostProcessSettings.bOverride_VignetteIntensity = false;
			RoguePlayer->camera->PostProcessSettings.bOverride_SceneFringeIntensity = false;
			RoguePlayer->camera->PostProcessSettings.bOverride_ChromaticAberrationStartOffset = false;
		}
		else
		{
			// 캐릭터 머티리얼 가져와서
			// 머티리얼 인덱스 0 부터 last까지 가져와서
			for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
			{
				// 기존 캐릭터 메쉬 머티리얼을 투명한 머티리얼로 바꾸자
				RoguePlayer->GetMesh()->SetMaterial(i, RoguePlayer->MatArr[i]);
			}
			// 무기 가지고 있으면
			if (RogueEquipComponent->GetSlots()[0] && weaponComponent->RogueItems)
			{
				for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
				{
					// 무기도 은신 머티리얼로 설정
					weaponComponent->RogueItems->SMComp->SetMaterial(i, weaponComponent->RogueItemSMMat[i]);
				}
			}
		}
	}
}

void UDBRogueSkillComponent::ActiveRogueESkill()
{
	// 쿨타임 다 차면 
	if (E_CurrCoolTime >= E_MaxCoolTime)
	{
		if (MyCharacter && MyCharacter->IsInventoryInViewport())
			return;

		ServerRPC_ActiveRogueESkill();
	}
}

// 서버에서 수리검 스폰한다
void UDBRogueSkillComponent::ServerRPC_ActiveRogueESkill_Implementation()
{
	UDBRogueAttackComponent* AttackComponent = GetOwner()->GetComponentByClass<UDBRogueAttackComponent>();
	if (AttackComponent && AttackComponent->IsInItemAction())
	{
		return;
	}

	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

	if (isSpawnKnife) return;
	if (RogueAnim->isCastingShift) return;

	isSpawnKnife = true;
	if (isSpawnKnife)
	{	// halfValue : 탄창갯수만큼 스폰된 칼의 중앙 위치 식 
		float halfValue = ((magazineCnt - 1) * 50) / 2.0f;
		// 탄창크기만큼 반복해서 총알 넣고 셋팅
		for (int32 i = 0; i < magazineCnt; i++)
		{
			// spawnActorDeferred : BeginPlay가 시작되기전에 실행됨
			ThrowingKnife = GetWorld()->SpawnActorDeferred<ARogueThrowingKnife>(ThrowingKnifeClass, RoguePlayer->ThrowKnifePos->GetComponentTransform());

			ThrowingKnife->SetOwner(GetOwner());
			TKMagazine.Add(ThrowingKnife);
			//스폰 시작
			UGameplayStatics::FinishSpawningActor(ThrowingKnife, RoguePlayer->ThrowKnifePos->GetComponentTransform());

			// 수리검의 인덱스를 수리검 갯수로 넘겨
			ThrowingKnife->KnifeNumber = i;
			// 중앙배치 식을 수리검에 넘기기 
			ThrowingKnife->halfValue = halfValue;
			ThrowingKnife->isThrowing = false;
		}
		MultiRPC_ActiveRogueESkill(isSpawnKnife);
	}
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueESkill_Implementation(bool isSpawn)
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	if (RoguePlayer->PlayerWidget == nullptr) return;
	if (RoguePlayer->IsLocallyControlled())
	{
		RoguePlayer->PlayerWidget->UpdateEBorder_Active(isSpawn);
	}
}

void UDBRogueSkillComponent::UpdateRogueESkill(float DeltaTime)
{
	// 탄창 배열 없으면
	if (TKMagazine.IsEmpty())
	{
		// 현재 타임을 맥스가 될때까지 더한다
		if (E_CurrCoolTime < E_MaxCoolTime)
		{
			E_CurrCoolTime += DeltaTime;
			//서버 플레이어를 위한 호출
			OnRep_CurrESkill();
		}
	}
}

void UDBRogueSkillComponent::ActiveRogueShiftSkill()
{
	if (MyCharacter && MyCharacter->IsInventoryInViewport())
		return;

	ServerRPC_ActiveRogueShiftSkill();
}

void UDBRogueSkillComponent::ServerRPC_ActiveRogueShiftSkill_Implementation()
{
	MultiRPC_ActiveRogueShiftSkill();
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueShiftSkill_Implementation()
{
	UDBRogueAttackComponent* AttackComponent = GetOwner()->GetComponentByClass<UDBRogueAttackComponent>();
	if (AttackComponent && AttackComponent->IsInItemAction())
	{
		return;
	}
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	if (RogueAnim->isCastingShift)return;
	if (!weaponComponent->hasWeapon) return;

	RogueAnim->isCastingShift = true;

	// 은신 상태면 은신 풀어주자
	if (isVanish)
	{
		DeactiveRogueQSkill();
	}
	if (weaponComponent->hasWeapon && RogueAnim->isCastingShift)
	{
		RoguePlayer->PlayAnimMontage(AM_RogueShiftSkill, 1.f, FName("Shift_Start"));
	}
}

