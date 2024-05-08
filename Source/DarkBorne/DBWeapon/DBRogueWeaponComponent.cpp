// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"

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
	//ÀåÂø ½½·Ô ¹è¿­ °¡Á®¿À±â
	EquipSlotArray = EquipComponent->GetSlots();

	hasWeapon = false;
}


// Called every frame
void UDBRogueWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	int32 index = 0;
	if (GetOwner()->HasAuthority())
	{
		auto Equip = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
		if (RogueItems && !RogueItems->IsPendingKill())
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("Testing Here: %s"), *GetNameSafe(RogueItems)));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Testing Here: Pending Kill")));
		}
	}
		
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
	// ¹«±â ²¨³»°í ÀÖÀ¸¸é Àç½ÇÇà x
	if (hasWeapon) return;
	hasWeapon = true;
	if (hasWeapon)
	{
		if (EquipSlotArray[0]) return;
		UDBEquipmentComponent* EquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
		UDBRogueSkillComponent* SkillComp = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();

		//ÀåÂø ½½·Ô ¹è¿­ °¡Á®¿À±â
		EquipSlotArray = EquipComponent->GetSlots();

		// ¹«±â½½·Ô¿¡ ¹«±âµ¥ÀÌÅÍ°¡ ÀÖÀ¸¸é
		if (EquipSlotArray[0])
		{
			// ¹«±â ¿ùµå¿¡ ½ºÆù delay
			// SpawnActorDeferred : BeginPlay°¡ ½ÇÇàµÇ±â Àü¿¡ ¼ÂÆÃ
			RogueItems = GetWorld()->SpawnActorDeferred<ADBItem>(EquipSlotArray[0]->GetItemClass(), GetComponentTransform(), GetOwner());
			RogueItemSMMat = RogueItems->SMComp->GetMaterials();
			// ¹«±âÀÇ ¸ÓÆ¼¸®¾ó °¡Á®¿À±â
			//RogueItems = GetWorld()->SpawnActor<ADBItem>(EquipSlotArray[0]->GetItemClass(), GetComponentLocation(), GetComponentRotation());

			//½ºÆù ½ÃÀÛ
			UGameplayStatics::FinishSpawningActor(RogueItems, GetComponentTransform());

			// ¹«±â¸¦ ÀÌ ÄÄÆ÷³ÍÆ®¿¡ ºÙÀÎ´Ù 
			RogueItems->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// ¹«±â ¿À³Ê ¼ÂÆÃ
			RogueItems->SetOwner(GetOwner());

			//다른 로직에 필요한 준비
			EquipSlotArray[0]->SetItemActor(RogueItems);
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

