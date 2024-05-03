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
	
}


// Called every frame
void UDBRogueSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateRogueQSkill(DeltaTime);
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
			//UE_LOG(LogTemp, Warning, TEXT("Vanish Curr Time : %.f"), CurrVanishTime);

			//���� ���Žð��� �ִ� ���Žð��� �������ٸ�
			if (CurrVanishTime >= MaxVanishTime)
			{
				//���� ���Žð� �ʱ�ȭ
				CurrVanishTime = 0;
				// ���� ��Ȱ��ȭ
				DeactiveRogueQSkill();
			}
		}
	}
}

void UDBRogueSkillComponent::ActiveRogueQSkill()
{
	//���� �Լ� ����
	ServerRPC_ActiveRogueQSkill();
}

void UDBRogueSkillComponent::ServerRPC_ActiveRogueQSkill_Implementation()
{
	// �������� Ŭ���̾�Ʈ�鿡�� �Ѹ���
	MultiRPC_ActiveRogueQSkill();
}

void UDBRogueSkillComponent::MultiRPC_ActiveRogueQSkill_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogue Q Skill"));
	//�α� ĳ���� �������� 
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	isVanish = true;
	// ������ ��� �ߴٸ�
	if (isVanish)
	{
		// ���� ���̶��
		if (RoguePlayer->IsLocallyControlled())
		{
			// ȭ�� ȸ�� ó��
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = true;
			RoguePlayer->camera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
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
		if (RoguePlayer->IsLocallyControlled())
		{
			//ȭ�� �������
			RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = false;
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
	ServerRPC_ActiveRogueESkill();
}

// �������� ������ �����Ѵ�
void UDBRogueSkillComponent::ServerRPC_ActiveRogueESkill_Implementation()
{
	if (isSpawnKnife) return;
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

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
	if (weaponComponent->hasWeapon)
	{
		RoguePlayer->PlayAnimMontage(AM_RogueShiftSkill, 1.f, FName("Shift_Start"));
	}
}

