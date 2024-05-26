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
	if (!ensureAlways(!GetItemActor())) return nullptr;

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

ADBItem* UItemObject::GetItemActor() const
{
	if (IsValid(ItemData.ItemActor))
	{
		return ItemData.ItemActor;
	}
	return nullptr;
}

void UItemObject::TryDestroyItemActor()
{
	AActor* ItemActor = GetItemActor();

	if (IsValid(ItemActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Actor %s, Destroying.."), *ItemActor->GetName());
		ItemData.ItemActor = nullptr;
		ItemActor->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Has no Actor"));
	}
}

FText UItemObject::GetDisplayName() const
{
	return ItemData.Item.SlotHolder.DisplayName;
}

FText UItemObject::GetLoreText() const
{
	return ItemData.Item.SlotHolder.LoreText;
}

EItemType UItemObject::GetItemType() const
{
	return ItemData.Item.SlotHolder.ItemType;
}

FName UItemObject::GetId() const
{
	return ItemData.Item.SlotHolder.Id;
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
		FText EffectText = ItemData.Item.SlotHolder.EffectText;
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

float UItemObject::GetRarityValue() const
{
	return ItemData.Item.GetDefaultValue();
}

FRarity UItemObject::GetRarity() const
{
	if (ensure(ItemData.Item.IsValid()))
	{
		TArray<FRarity> Rarities = ItemData.Item.GetRarities();
		return Rarities[0];
	}
	return FRarity();
}

FDarkBorneStats UItemObject::GetEnchantments() const
{
	return ItemData.Item.Enchantments;
}

FIntPoint UItemObject::GetDimentions() const
{
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	//return _dimentions;
	FIntPoint Temp(ItemData.Item.SlotHolder.SlotDimension.X, ItemData.Item.SlotHolder.SlotDimension.Y);
	return Temp;
}

UMaterialInterface* UItemObject::GetIcon() const
{
	return ItemData.Item.SlotHolder.DisplayMaterial;
}

UTexture2D* UItemObject::GetIconTexture() const
{
	return ItemData.Item.SlotHolder.IconTexture;
}

TSubclassOf<AActor> UItemObject::GetItemClass() const
{
	return ItemData.Item.SlotHolder.ItemClass;
}

ESlotType UItemObject::GetSlotType() const
{
	return ItemData.Item.SlotHolder.SlotType;
}

const FItem& UItemObject::GetItem() const
{
	return ItemData.Item;
}

//void UItemObject::Server_SpawnItem_Implementation(AActor* Initiator, float forwardOffset, bool bSetOwner)
//{
//	///UE_LOG(LogTemp, Warning, TEXT("Net? %s"),
//		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER")
//	);
//	if (Initiator && GetWorld()) {
//		FVector SpawnLoc = Initiator->GetActorLocation();
//		SpawnLoc += Initiator->GetActorForwardVector() * forwardOffset;
//		FTransform Trans;
//		Trans.SetLocation(SpawnLoc);
//		Trans.SetRotation(FQuat::Identity);
//		Trans.SetScale3D(FVector::OneVector);
//
//		auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);
//
//		ItemSpawned->Initialize(this);
//
//		UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Spawn Item Failed in %s"), *GetNameSafe(this));
//		return;
//	}
//}

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

TStatId UItemObject::GetStatId() const
{
	return TStatId();
}

void UItemObject::Tick(float DeltaTime)
{
	//if (GetWorld())
	//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf
	//	(
	//		TEXT("[%s] %s"),
	//		(GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
	//		*ItemData.Item.SlotHolder.DisplayName.ToString()
	//	)
	//	);
}

void UItemObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, ItemData);
}
