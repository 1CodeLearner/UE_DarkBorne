// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

#include "../DBCharacters/DBRogueCharacter.h"
#include "../Items/Weapons/DBWeapon_CloseRange.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"
#include "../DBCharacters/DBCharacterAttack/DBRogueAttackComponent.h"

#include "../Framework/Interfaces/ItemInterface.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "../DBCharacters/DBCharacterAttack/DBRogueAttackComponent.h"
#include "../Status/CharacterStatusComponent.h"
#include "../DBPlayerWidget/DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

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

	AttackComp = GetOwner()->GetComponentByClass<UDBRogueAttackComponent>();
}


// Called every frame
void UDBRogueWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	int32 index = 0;
	if (GetOwner()->HasAuthority())
	{
		auto Equip = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
		if (RogueItems && IsValid(RogueItems))
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
	enhancedInputComponent->BindAction(ia_ConsumableSlot, ETriggerEvent::Triggered, this, &UDBRogueWeaponComponent::AttachConsumable);
}

void UDBRogueWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UDBRogueWeaponComponent, RogueItems);
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

	if (AttackComp && AttackComp->IsUsingItem()) return;

	if (!hasWeapon)
	{
		hasWeapon = HandleAttach(UItemLibrary::GetSlotIndexByEnum(ESlotType::WEAPON));
		if (RogueItems != nullptr)
		{
		hasWeapon = true;
		UCharacterStatusComponent::AdjustAddedStats(GetOwner(), RogueItems->GetItemObject(), hasWeapon);
		
		}
		else
		{
		hasWeapon = false;

		}
		// 서버 플레이어를 위한 호출
		//OnRep_hasWeapon();

	}
}

void UDBRogueWeaponComponent::PassItem(UItemObject* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Passing Item to DBRogueWeaponComponent: %s"), *GetNameSafe(Item));
	if (Item->GetSlotType() == ESlotType::WEAPON)
	{
		hasWeapon = false;
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
	case ESlotType::CONSUMABLE:
		if (RogueItems && RogueItems->GetItemObject()->GetSlotType() == Item->GetSlotType())
			AttachConsumable();
		break;
	}
}
// 슬롯에서 아이템 떨어졌을때
void UDBRogueWeaponComponent::TryRemoveRogueItem(UItemObject* Item)
{
	if (Item && RogueItems && Item->GetItemActor() == RogueItems)
	{
		RogueItems->GetItemObject()->TryDestroyItemActor();
		RogueItems = nullptr;

		if (Item->GetSlotType() == ESlotType::WEAPON) 
		{
			hasWeapon = false;
		}
		
	}
}



void UDBRogueWeaponComponent::AttachConsumable()
{
	Server_AttachConsumable();
}

void UDBRogueWeaponComponent::Server_AttachConsumable_Implementation()
{
	//if player is not in a middle of an attack sequence, switch to consumable item.
	if (AttackComp && AttackComp->comboCnt == 0)
	{
		if (HandleAttach(UItemLibrary::GetSlotIndexByEnum(ESlotType::CONSUMABLE)))
		{
			hasWeapon = false;			
		}
	}
}




//Return true if either item attachment is successful or player is already holding the same item. Returns false if EquipSlotArray is invalid, or slot is empty
bool UDBRogueWeaponComponent::HandleAttach(int32 SlotIndex)
{
	UDBEquipmentComponent* EquipComponent = GetOwner()->GetComponentByClass<UDBEquipmentComponent>();
	UDBRogueSkillComponent* SkillComp = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();

	//ÀåÂø ½½·Ô ¹è¿­ °¡Á®¿À±â
	TArray<UItemObject*> Slots = EquipComponent->GetSlots();

	// ¹«±â½½·Ô¿¡ ¹«±âµ¥ÀÌÅÍ°¡ ÀÖÀ¸¸é
	if (ensureAlways(!Slots.IsEmpty()) && Slots[SlotIndex])
	{
		if (Slots[SlotIndex]->GetItemActor() == nullptr)
		{
			if (RogueItems) 
			{
				if (RogueItems->GetItemObject()->GetSlotType() == ESlotType::WEAPON)
				{
					UCharacterStatusComponent::AdjustAddedStats(GetOwner(), RogueItems->GetItemObject(), false);
				}

				RogueItems->GetItemObject()->TryDestroyItemActor();
			}

			// ¹«±â ¿ùµå¿¡ ½ºÆù delay
			// SpawnActorDeferred : BeginPlay°¡ ½ÇÇàµÇ±â Àü¿¡ ¼ÂÆÃ
			RogueItems = GetWorld()->SpawnActorDeferred<ADBItem>(Slots[SlotIndex]->GetItemClass(), GetComponentTransform(), GetOwner());
			RogueItemSMMat = RogueItems->SMComp->GetMaterials();
			RogueItems->Initialize(Slots[SlotIndex]);

			// ¹«±âÀÇ ¸ÓÆ¼¸®¾ó °¡Á®¿À±â
			//RogueItems = GetWorld()->SpawnActor<ADBItem>(Slots[0]->GetItemClass(), GetComponentLocation(), GetComponentRotation());

			//½ºÆù ½ÃÀÛ
			UGameplayStatics::FinishSpawningActor(RogueItems, GetComponentTransform());

			// ¹«±â¸¦ ÀÌ ÄÄÆ÷³ÍÆ®¿¡ ºÙÀÎ´Ù 
			RogueItems->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// ¹«±â ¿À³Ê ¼ÂÆÃ
			RogueItems->SetOwner(GetOwner());

			//다른 로직에 필요한 준비
			Slots[SlotIndex]->SetItemActor(RogueItems);


			//OnRep_RogueItems();
		}

		return true;
	}

	return false;
}
