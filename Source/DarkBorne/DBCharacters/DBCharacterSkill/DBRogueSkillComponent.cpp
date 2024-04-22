// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "../../Items/Weapons/DBWeapon_CloseRange.h"

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


	// ...
	
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

}



void UDBRogueSkillComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	

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
			UE_LOG(LogTemp, Warning, TEXT("Vanish Curr Time : %.f"), CurrVanishTime);
		
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
	if (isVanish == true)
	{
		// ȭ�� ȸ�� ó��
		RoguePlayer->camera->PostProcessSettings.bOverride_ColorSaturation = true;
		RoguePlayer->camera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
		// �α� ĳ���� ��Ƽ���� �ε��� 0 ���� last���� �����ͼ�
		for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
		{

			// ���� ĳ���� �޽� ��Ƽ������ ������ ��Ƽ����� �ٲ���
			RoguePlayer->GetMesh()->SetMaterial(i, VanishMat);
		}
		// ���� ������ ������
		if (weaponComponent != nullptr)
		{
			// ���� ��Ƽ���� �ε��� �����ͼ�
			for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
			{
				// ���⸦ ���� ��Ƽ����� ����
				// dagger�� ���������� ����
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
		//UE_LOG(LogTemp, Warning, TEXT("off vanish"));
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
		if (weaponComponent != nullptr)
		{

			for (int32 i = 0; i < weaponComponent->RogueItemSMMat.Num(); i++)
			{
				// ���⵵ ���� ��Ƽ����� ����
				weaponComponent->RogueItems->SMComp->SetMaterial(i, weaponComponent->RogueItemSMMat[i]);
			}
		}

	}
}

