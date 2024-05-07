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

// Sets default values for this component's properties
UDBRogueSkillComponent::UDBRogueSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UDBRogueSkillComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Q_CurrCoolTime = Q_MaxCoolTime;
	E_CurrCoolTime = E_MaxCoolTime;
	
	OnRep_CurrQSkill();
	OnRep_CurrESkill();
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
	
	// �÷��̾� ������ ������ ����
	if (RoguePlayer->PlayerWidget == nullptr) return;
	RoguePlayer->PlayerWidget->UpdateQSkillBar(Q_CurrCoolTime, Q_MaxCoolTime);
	//UE_LOG(LogTemp, Warning, TEXT("Testing:%f"), CurrHP);
}

void UDBRogueSkillComponent::OnRep_CurrESkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());

	// �÷��̾� ������ ������ ����
	if (RoguePlayer->PlayerWidget == nullptr) return;
	RoguePlayer->PlayerWidget->UpdateESkillBar(E_CurrCoolTime, E_MaxCoolTime);
}

void UDBRogueSkillComponent::UpdateRogueQSkill(float DeltaTime)
{
	if (isVanish)
	{
		// ���� ���Žð��� �ִ�ð����� �۴ٸ�
		if (CurrVanishTime < MaxVanishTime)
		{
			// ���� ���� �ð��� ��ŸŸ�� ���ϱ�
			CurrVanishTime += DeltaTime;

			//���� ���Žð��� �ִ� ���Žð��� �������ٸ�
			if (CurrVanishTime >= MaxVanishTime)
			{	
				// ���� ��Ȱ��ȭ
				DeactiveRogueQSkill();
				
			}
		}
	}
	// ���� false�� ���� ��Ÿ���� �ִ� �𺸴� �۴ٸ�
	else if (!isVanish && Q_CurrCoolTime < Q_MaxCoolTime)
	{
		// curr cooltime�� ��ŸŸ�� ���Ѵ�
		Q_CurrCoolTime += DeltaTime;

		//���� �÷��̾ ���� ȣ��
		OnRep_CurrQSkill();

		//UE_LOG(LogTemp, Warning, TEXT("Vanish Cool Time : %.f"), Q_CurrCoolTime);
	}
}

void UDBRogueSkillComponent::ActiveRogueQSkill()
{
	// curr cooltime >= max time�� �Ǿ��ٸ�
	if (Q_CurrCoolTime >= Q_MaxCoolTime)
	{
		// q skill Ȱ��ȭ
		//���� �Լ� ����
		ServerRPC_ActiveRogueQSkill();
	}
}

void UDBRogueSkillComponent::ServerRPC_ActiveRogueQSkill_Implementation()
{
	// �������� Ŭ���̾�Ʈ�鿡�� �Ѹ���
	MultiRPC_ActiveRogueQSkill();
	
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueQSkill_Implementation()
{
	//�α� ĳ���� �������� 
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();
	
	if (RogueAnim->isCastingShift) return;
	if(isVanish) return;
	isVanish = true;
	// ������ ��� �ߴٸ�
	UE_LOG(LogTemp, Warning, TEXT("Rogue Q Skill"));
	if (isVanish)
	{
		// ���� ���̶��
		if (RoguePlayer->IsLocallyControlled())
		{
			// ȭ�� ȸ�� ó��
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = true;
			RoguePlayer->camera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);

			RoguePlayer->camera->PostProcessSettings.bOverride_VignetteIntensity = true;
			RoguePlayer->camera->PostProcessSettings.VignetteIntensity = 0.7f;
		}
		// �ٸ� ĳ���͵�����
		else
		{
			// �α� ĳ���� ��Ƽ���� �ε��� 0 ���� last���� �����ͼ�
			for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
			{
				//UE_LOG(LogTemp, Warning, TEXT("PlayerMat"));
				// ���� ĳ���� �޽� ��Ƽ������ ������ ��Ƽ����� �ٲ���
				RoguePlayer->GetMesh()->SetMaterial(i, VanishMat);
			}
			// ���� ������ ������
			if (weaponComponent->EquipSlotArray[0])
			{
				// ���� ��Ƽ���� �ε��� �����ͼ�
				for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
				{
					// ���⸦ ���� ��Ƽ����� ����
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

	isVanish = false;
	
	// �ִ� ���ӽð��� �����ߴٸ�
	if (!isVanish)
	{	
		//���� �ð�, ���� ��Ÿ�� �ʱ�ȭ
		CurrVanishTime = 0;
		Q_CurrCoolTime = 0;
		
		if (RoguePlayer->IsLocallyControlled())
		{
			//ȭ�� �������
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = false;
			RoguePlayer->camera->PostProcessSettings.bOverride_VignetteIntensity = false;
		}
		else
		{
			//�α� ĳ���� ��Ƽ���� �����ͼ�
			// ��Ƽ���� �ε��� 0 ���� last���� �����ͼ�
			for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
			{
				// ���� ĳ���� �޽� ��Ƽ������ ������ ��Ƽ����� �ٲ���
				RoguePlayer->GetMesh()->SetMaterial(i, RoguePlayer->MatArr[i]);
			}
			// ���� ������ ������
			if (weaponComponent->EquipSlotArray[0])
			{
				for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
				{
					// ���⵵ ���� ��Ƽ����� ����
					weaponComponent->RogueItems->SMComp->SetMaterial(i, weaponComponent->RogueItemSMMat[i]);
				}
			}
		}
	}
}


void UDBRogueSkillComponent::ActiveRogueESkill()
{
	// ��Ÿ�� �� ���� 
	if (E_CurrCoolTime >= E_MaxCoolTime)
	{
		ServerRPC_ActiveRogueESkill();
	}
}

// �������� ������ �����Ѵ�
void UDBRogueSkillComponent::ServerRPC_ActiveRogueESkill_Implementation()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

	if (isSpawnKnife) return;
	if(RogueAnim->isCastingShift) return;

	isSpawnKnife = true;
	if (isSpawnKnife)
	{
		float halfValue = ((magazineCnt - 1) * 50) / 2.0f;

		for (int32 i = 0; i < magazineCnt; i++)
		{
			//FVector NewLoc = RoguePlayer->ThrowKnifePos->GetComponentLocation();
			//FRotator NewRot = RoguePlayer->ThrowKnifePos->GetForwardVector().Rotation();
			//NewRot.Normalize();
			ThrowingKnife = GetWorld()->SpawnActorDeferred<ARogueThrowingKnife>(ThrowingKnifeClass, RoguePlayer->ThrowKnifePos->GetComponentTransform());

			ThrowingKnife->SetOwner(GetOwner());
			TKMagazine.Add(ThrowingKnife);
			//���� ����
			UGameplayStatics::FinishSpawningActor(ThrowingKnife, RoguePlayer->ThrowKnifePos->GetComponentTransform());
			//ThrowingKnife = GetWorld()->SpawnActor<ARogueThrowingKnife>(ThrowingKnifeClass, NewLoc, NewRot);
			
			// �������� �ε����� ������ ������ �Ѱ�
			ThrowingKnife->KnifeNumber = i;
			// �߾ӹ�ġ ���� �����˿� �ѱ�� 
			ThrowingKnife->halfValue = halfValue;
			ThrowingKnife->isThrowing = false;
			
		}

	}

}

void UDBRogueSkillComponent::UpdateRogueESkill(float DeltaTime)
{
	// źâ �迭�� isEmpty ���
	if (TKMagazine.IsEmpty())
	{	
		// ���� Ÿ���� �ƽ��� �ɶ����� ���Ѵ�
		if (E_CurrCoolTime < E_MaxCoolTime)
		{
			E_CurrCoolTime += DeltaTime;
			//���� �÷��̾ ���� ȣ��
			OnRep_CurrESkill();
			UE_LOG(LogTemp, Warning, TEXT("throw skill cool is : %.f"), E_CurrCoolTime);
		}
	}
}

void UDBRogueSkillComponent::ActiveRogueShiftSkill()
{
	ServerRPC_ActiveRogueShiftSkill();
}

void UDBRogueSkillComponent::ServerRPC_ActiveRogueShiftSkill_Implementation()
{
	MultiRPC_ActiveRogueShiftSkill();
	
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueShiftSkill_Implementation()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	if(RogueAnim->isCastingShift) return;
	if(!weaponComponent->hasWeapon) return;
		RogueAnim->isCastingShift = true;

	// ���� ���¸� ���� Ǯ������
	if (isVanish)
	{
		DeactiveRogueQSkill();
	}

	if (weaponComponent->hasWeapon && RogueAnim->isCastingShift)
	{
		RoguePlayer->PlayAnimMontage(AM_RogueShiftSkill, 1.f, FName("Shift_Start"));
	}
}

