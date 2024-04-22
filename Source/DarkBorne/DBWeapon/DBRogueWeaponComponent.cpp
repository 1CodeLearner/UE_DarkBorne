// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../Inventory/DBEquipmentComponent.h"

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
	ServerRPC_AttachWeapon();
}

void UDBRogueWeaponComponent::ServerRPC_AttachWeapon_Implementation()
{
	MultiRPC_AttachWeapon();
}

void UDBRogueWeaponComponent::MultiRPC_AttachWeapon_Implementation()
{
	// 무기 있으면 재실행 x
	if (hasWeapon) return;

	hasWeapon = true;
	if (hasWeapon)
	{

	UDBEquipmentComponent* EquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
	//장착 슬롯 배열 가져오기
	EquipSlotArray = EquipComponent->GetSlots();

		// 무기슬롯에 무기데이터가 있으면
		if (EquipSlotArray[0])
		{
			// 무기 월드에 스폰
			RogueItems = GetWorld()->SpawnActor<ADBItem>(EquipSlotArray[0]->GetItemClass(), GetComponentLocation(), GetComponentRotation());

			//이 무기의 오너를 셋팅 
			RogueItems->SetOwner(GetOwner());
			RogueItems->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			RogueItemSMMat = RogueItems->SMComp->GetMaterials();
			
		}
	}

	//if (hasWeapon)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Attach Weapon"));
	//	Dagger = GetWorld()->SpawnActor<ADBWeapon_CloseRange>(DaggerFactory, GetComponentLocation(), GetComponentRotation());
	//	if (Dagger)
	//	{
	//
	//		//이 무기의 오너를 셋팅 
	//		Dagger->SetOwner(GetOwner());
	//
	//		// 단검을 붙이자
	//		Dagger->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//
	//		// 단검 메쉬의 머터리얼을 가져오자
	//		DaggerSMMat = Dagger->SMComp->GetMaterials();
	//		//UE_LOG(LogTemp, Warning, TEXT("WeaponComp Index : %d"), DaggerSMMat.Num());
	//	}
	//	
	//
	//}

}

void UDBRogueWeaponComponent::PassItem(UItemObject* Item)
{
	if (Item->GetSlotType() == ESlotType::WEAPON)
	{
		AttachWeapon();
	}

	switch (Item->GetSlotType()) 
	{
		case ESlotType::WEAPON:
		break;
		case ESlotType::HEAD:
		break;
		case ESlotType::UPPERWEAR:
		break;
		case ESlotType::BOTTOMWEAR:
		break;
		case ESlotType::GLOVES:
		break;
		case ESlotType::BOOTS:
		break;

	}
}

