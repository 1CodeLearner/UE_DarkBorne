// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UDBRogueWeaponComponent::UDBRogueWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UDBRogueWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	UDBEquipmentComponent* EquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
	//장착 슬롯 배열 가져오기
	EquipSlotArray = EquipComponent->GetSlots();
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

void UDBRogueWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UDBRogueWeaponComponent, RogueItems);
	DOREPLIFETIME(UDBRogueWeaponComponent, EquipSlotArray);
	DOREPLIFETIME(UDBRogueWeaponComponent, RogueItemSMMat);
	DOREPLIFETIME(UDBRogueWeaponComponent, Dagger);
	DOREPLIFETIME(UDBRogueWeaponComponent, hasWeapon);
}

	
void UDBRogueWeaponComponent::AttachWeapon()
{
	ServerRPC_AttachWeapon();
}

void UDBRogueWeaponComponent::ServerRPC_AttachWeapon_Implementation()
{
	// 무기 꺼내고 있으면 재실행 x
	if (hasWeapon) return;
	
	hasWeapon = true;
	if (hasWeapon)
	{
		if(EquipSlotArray[0]) return;
		UDBEquipmentComponent* EquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
		//장착 슬롯 배열 가져오기
		EquipSlotArray = EquipComponent->GetSlots();
		
		// 무기슬롯에 무기데이터가 있으면
		if (EquipSlotArray[0])
		{
			// 무기 월드에 스폰 delay
			// SpawnActorDeferred : BeginPlay가 실행되기 전에 셋팅
			RogueItems = GetWorld()->SpawnActorDeferred<ADBItem>(EquipSlotArray[0]->GetItemClass(), GetComponentTransform(), GetOwner());
			//RogueItems = GetWorld()->SpawnActor<ADBItem>(EquipSlotArray[0]->GetItemClass(), GetComponentLocation(), GetComponentRotation());

			//스폰 시작
			UGameplayStatics::FinishSpawningActor(RogueItems, GetComponentTransform());

			// 무기를 이 컴포넌트에 붙인다 
			RogueItems->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// 무기의 머티리얼 가져오기
			RogueItemSMMat = RogueItems->SMComp->GetMaterials();
			// 무기 오너 셋팅
			RogueItems->SetOwner(GetOwner());

		}
	
	}
	

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

