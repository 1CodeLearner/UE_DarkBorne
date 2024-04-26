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

// Sets default values for this component's properties
UDBRogueSkillComponent::UDBRogueSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
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

}



void UDBRogueSkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBRogueSkillComponent, isVanish);
	DOREPLIFETIME(UDBRogueSkillComponent, MaxVanishTime);
	DOREPLIFETIME(UDBRogueSkillComponent, CurrVanishTime);


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
		// ȭ�� ȸ�� ó��
		RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = true;
		RoguePlayer->camera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
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

void UDBRogueSkillComponent::DeactiveRogueQSkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueWeaponComponent* weaponComponent = GetOwner()->GetComponentByClass<UDBRogueWeaponComponent>();

	isVanish = false;
	// �ִ� ���ӽð��� �����ߴٸ�
	if (!isVanish)
	{
		//ȭ�� �������
		RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = false;
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

void UDBRogueSkillComponent::UpdateRogueESkill(float DeltaTime)
{

}

void UDBRogueSkillComponent::ActiveRogueESkill()
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

	
	GetWorld()->SpawnActor<ARogueThrowingKnife>(ThrowKnifeFactory, RoguePlayer->GetActorLocation(), RoguePlayer->GetActorRotation());
	//�������� ���� ����
	ThrowingKnife->SetOwner(GetOwner());
	
	//������ �迭�� ������ 4�� ����
	//ThrowKnifeArray.Init(ThrowingKnife, 4);
	
	//for (int32 i = 0; i < ThrowKnifeArray.Num(); i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("shook"));
	//}

	//if (ThrowingKnife != nullptr)
	//{
	//	ThrowingKnife->PlayMontage(RoguePlayer, FName("ESkill_Start"));
	//}

	
}

