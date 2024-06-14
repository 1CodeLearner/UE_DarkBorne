// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "../Items/PDA_ItemSlot.h"
#include "Materials/MaterialInterface.h"
#include "../Items/DBItem.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "../Items/DBItem.h"
#include "../Framework/Interfaces/ItemInterface.h"

void UItemObject::Initialize(FItem _Item, ADBItem* _ItemActor)
{
	ItemData = FItemData(_Item, _ItemActor);
}

ADBItem* UItemObject::SpawnItem(AActor* Initiator, bool bSetOwner, FTransform Trans, float forwardOffset)
{
	if (!ensureAlways(!ItemData.ItemActor)) return nullptr;

	auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);
	ItemSpawned->Initialize(this);
	UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);

	SetItemActor(ItemSpawned);

	return ItemSpawned;
}

bool UItemObject::ImplementsItemInterface() const
{
	return GetItemClass()->ImplementsInterface(UItemInterface::StaticClass());
}

void UItemObject::SetItemActor(ADBItem* _ItemActor)
{
	if (ensureAlways(_ItemActor))
		ItemData.ItemActor = _ItemActor;
}

bool UItemObject::HasItemActor() const
{
	return IsValid(ItemData.ItemActor);
}

bool UItemObject::HasItemActor(ADBItem* ItemToCheck) const
{
	return ItemToCheck && IsValid(ItemData.ItemActor) && ItemData.ItemActor == ItemToCheck;
}

void UItemObject::TryDestroyItemActor()
{
	if (IsValid(ItemData.ItemActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Actor %s, Destroying.."), *ItemData.ItemActor->GetName());
		AActor* TempActor = ItemData.ItemActor;
		ItemData.ItemActor = nullptr;
		TempActor->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Has no Actor"));
	}
}

FText UItemObject::GetDisplayName() const
{
	return ItemData.Item.GetSlotHolder().DisplayName;
}

FText UItemObject::GetLoreText() const
{
	return ItemData.Item.GetSlotHolder().LoreText;
}

EItemType UItemObject::GetItemType() const
{
	return ItemData.Item.GetSlotHolder().ItemType;
}

FName UItemObject::GetId() const
{
	return ItemData.Item.GetSlotHolder().Id;
}

FText UItemObject::GetBaseStatsText() const
{
	float BaseStat = GetRarityValue();
	FText BaseStatText;

	switch (GetItemType())
	{
	case EItemType::WEAPON:
	{
		BaseStatText = FText::FromString(FString::Printf(TEXT("Weapon Damage %.0f"), BaseStat));
		break;
	}
	case EItemType::CONSUMABLE:
	{
		break;
	}
	}

	return BaseStatText;
}

TArray<FText> UItemObject::GetEnchantmentsTexts() const
{
	FDarkBorneStats Enchantments = GetEnchantments();
	TArray<FText> EnchantmentsTexts;

	switch (GetItemType())
	{
	case EItemType::WEAPON:
	{
		const TArray<FAttribute> Attributes = Enchantments.Attributes;
		for (int i = 0; i < Attributes.Num(); ++i)
		{
			FText AttributeTypeText;
			EAttributeType AttributeType = Attributes[i].AttributeType;
			UEnum::GetDisplayValueAsText(AttributeType, AttributeTypeText);

			FText AttributeValueText;
			float RangeValue = Attributes[i].Range.min;
			AttributeValueText = FText::FromString(FString::Printf(TEXT("%.0f"), RangeValue));

			FText FinalText = FText::Format(FText::FromString("{0} {1}"), AttributeValueText, AttributeTypeText);

			EnchantmentsTexts.Add(FinalText);
		}

		const TArray<FPhysicalDamage> PhysicalDamages = Enchantments.PhysicalDamages;
		for (int i = 0; i < PhysicalDamages.Num(); ++i)
		{
			EPhysicalDamageType PhysicalDamageType = PhysicalDamages[i].PhysicalDamageType;
			FText PhysicalDamageTypeText;
			UEnum::GetDisplayValueAsText(PhysicalDamageType, PhysicalDamageTypeText);

			FText PhysicalDamageValueText;
			float RangeValue = PhysicalDamages[i].Range.min;
			PhysicalDamageValueText = FText::FromString(FString::Printf(TEXT("%.0f"), RangeValue));
			
			FText FinalText = FText::Format(FText::FromString("{0}% {1}"), PhysicalDamageValueText, PhysicalDamageTypeText);

			EnchantmentsTexts.Add(FinalText);
		}
		break;
	}
	case EItemType::CONSUMABLE:
	{
		break;
	}
	}

	return EnchantmentsTexts;
}

FText UItemObject::GetEffectText() const
{
	float EffectValue = GetRarityValue();
	FText EffectValueText;

	switch (GetItemType())
	{
	case EItemType::WEAPON:
	{
		break;
	}
	case EItemType::CONSUMABLE:
	{
		FText EffectText = ItemData.Item.GetSlotHolder().EffectText;
		if (!EffectText.IsEmpty()) 
		{
			FString EffectString = EffectText.ToString();
			FString FindString = TEXT("%f");
			int32 index = EffectString.Find(FindString);
			if (index != -1) 
			{
				EffectString = EffectString.Replace(*FindString, *FString::SanitizeFloat(EffectValue, 0));
				EffectValueText = FText::FromString(EffectString);
			}
			else 
			{
				EffectValueText = EffectText;
			}
		}
		break;
	}
	}

	return EffectValueText;
}

FText UItemObject::GetCategoryText() const
{
	ESlotType SlotType = GetSlotType();
	FText SlotTypeText;
	UEnum::GetDisplayValueAsText(SlotType, SlotTypeText);

	FText CategoryText = FText::FromString(FString::Printf(TEXT("Slot type: %s"), *SlotTypeText.ToString()));
	return CategoryText;
}

USoundBase* UItemObject::GetEquipSound() const
{
	return ItemData.Item.GetSlotHolder().EquipSound;
}

USoundBase* UItemObject::GetInventorySound() const
{
	return ItemData.Item.GetSlotHolder().InventorySound;
}

USoundBase* UItemObject::GetDropSound() const
{
	return ItemData.Item.GetSlotHolder().DropSound;
}

float UItemObject::GetRarityValue() const
{
	return ItemData.Item.GetDefaultValue();
}

FRarity UItemObject::GetRarity() const
{
	if (ensure(ItemData.Item.IsValid()))
	{
		return ItemData.Item.GetRarity();
	}
	return FRarity();
}

FDarkBorneStats UItemObject::GetEnchantments() const
{
	return ItemData.Item.GetEnchantments();
}

FIntPoint UItemObject::GetDimensions() const
{
	FIntPoint Dimensions(ItemData.Item.GetSlotHolder().SlotDimension.X, ItemData.Item.GetSlotHolder().SlotDimension.Y);
	return Dimensions;
}

UMaterialInterface* UItemObject::GetIcon() const
{
	return ItemData.Item.GetSlotHolder().DisplayMaterial;
}

UTexture2D* UItemObject::GetIconTexture() const
{
	return ItemData.Item.GetSlotHolder().IconTexture;
}

TSubclassOf<AActor> UItemObject::GetItemClass() const
{
	return ItemData.Item.GetSlotHolder().ItemClass;
}

ESlotType UItemObject::GetSlotType() const
{
	return ItemData.Item.GetSlotHolder().SlotType;
}

const FItem& UItemObject::GetItemData() const
{
	return ItemData.Item;
}

UWorld* UItemObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

void UItemObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, ItemData);
}
