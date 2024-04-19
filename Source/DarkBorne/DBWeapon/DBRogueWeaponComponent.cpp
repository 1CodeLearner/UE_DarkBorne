// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"

// Sets default values for this component's properties
UDBRogueWeaponComponent::UDBRogueWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDBRogueWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	hasWeapon = false;
}


// Called every frame
void UDBRogueWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDBRogueWeaponComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_WeaponSlot, ETriggerEvent::Triggered, this, &UDBRogueWeaponComponent::AttachWeapon);
		
}

void UDBRogueWeaponComponent::AttachWeapon()
{
	// ���� ������ ����� x
	if(hasWeapon) return;

	hasWeapon = true;
	if (hasWeapon)
	{

		UE_LOG(LogTemp, Warning, TEXT("Attach Weapon"));
		Dagger = GetWorld()->SpawnActor<ADBWeapon_CloseRange>(DaggerFactory, GetComponentLocation(), GetComponentRotation());
		if (Dagger)
		{
			//ADBRogueCharacter* player = Cast<ADBRogueCharacter>(GetOwner());
			//Dagger->SetOwner(player);
			//�� ������ ���ʸ� ���� 
			Dagger->SetOwner(GetOwner());

			// �ܰ��� ������
			Dagger->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
			// �ܰ� �޽��� ���͸����� ��������
			DaggerSMMat = Dagger->SMComp->GetMaterials();
			UE_LOG(LogTemp, Warning, TEXT("WeaponComp Index : %d"), DaggerSMMat.Num());
		}
		
	}
	
}

