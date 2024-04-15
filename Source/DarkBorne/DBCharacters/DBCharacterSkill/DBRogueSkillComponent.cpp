// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include "../../Items/Weapons/DBWeapon.h"

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
	isVanish = false;
}


// Called every frame
void UDBRogueSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDBRogueSkillComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_Q_Skill, ETriggerEvent::Triggered, this, &UDBRogueSkillComponent::RogueQSkill);

}

void UDBRogueSkillComponent::RogueQSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogue Q Skill"));
	//로그 캐릭터 가져오기 
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueWeaponComponent* weaponComponent = Cast<UDBRogueWeaponComponent>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("SkillComp Index : %d"), weaponComponent->DaggerSMMat.Num());
	
	isVanish = true;
	if (isVanish)
	{
		//로그 캐릭터 머티리얼 가져와서
		// 머티리얼 인덱스 0 부터 last까지 가져와서
		for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
		{

			// 기존 캐릭터 메쉬 머티리얼을 투명한 머티리얼로 바꾸자
			RoguePlayer->GetMesh()->SetMaterial(i, VanishMat);
		}
		// 
		if (weaponComponent != nullptr)
		{
		
			for (int32 i = 0; i < weaponComponent->DaggerSMMat.Num(); i++)
			{
				
				weaponComponent->Dagger->SMComp->SetMaterial(i, VanishMat);
			}
		}
	}
	else if (!isVanish)
	{

	}
	
	

}

