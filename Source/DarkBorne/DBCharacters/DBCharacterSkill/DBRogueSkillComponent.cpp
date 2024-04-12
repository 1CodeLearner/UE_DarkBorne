// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"

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

	// ...
}

void UDBRogueSkillComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_Q_Skill, ETriggerEvent::Triggered, this, &UDBRogueSkillComponent::RogueQSkill);

}

void UDBRogueSkillComponent::RogueQSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogue Q Skill"));

	//�α� ĳ���� �������� 
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	//�α� ĳ���� ��Ƽ���� �����ͼ�
	// ��Ƽ���� �ε��� 0 ���� last���� �����ͼ�
	for (int32 i = 0; i < RoguePlayer->MatArr.Num(); i++)
	{

		// ���� ĳ���� �޽� ��Ƽ������ ������ ��Ƽ����� �ٲ���
		RoguePlayer->GetMesh()->SetMaterial(i, VanishMat);
	}
	
	// set material�� ���� ������ ��Ƽ����� ��Ƽ���� �ε��� ����
	

}

