// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMainWidget.h"
#include "BaseInventoryComponent.h"
#include "PlayerEquipmentComponent.h"
#include "DBEquipmentComponent.h"
#include "LootInventoryComponent.h"

#include "../Inventory/InventoryGridWidget.h"
#include "EquipmentGridWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../ItemTypes/EnchantmentTypes.h"

void UInventoryMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
	HideLoots();
}

void UInventoryMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Equipment Loot: %s"),
		*GetNameSafe(EquipmentComp_Loot)));

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Equipment Loot: %s"),
		*GetNameSafe(InventoryComp_Loot)));

}

void UInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryMainWidget::DisplayInventory(bool bEnabled)
{
	if (bEnabled)
	{
		if (IsLootValid())
		{
			if (EquipmentComp) //Dead Player's Inventory
			{
				DisplayPlayerLoot(InventoryComp_Loot, EquipmentComp_Loot);
			}
			else // non-player inventory (Chests, monsters)
			{
				DisplayOtherLoot(InventoryComp_Loot);
			}
		}

		if (ensureAlways(!IsInViewport()))
		{
			AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
				GetOwningPlayer(), this, EMouseLockMode::LockOnCapture);
			GetOwningPlayer()->bShowMouseCursor = true;
		}
	}
	else
	{
		if (IsLootValid())
		{
			ClearLoot();
			HideLoots();
			bLootValid = false;
		}

		if (ensureAlways(IsInViewport()))
		{
			RemoveFromParent();
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer(), true);
			GetOwningPlayer()->bShowMouseCursor = false;
		}
	}
}

void UInventoryMainWidget::InitLootDisplay(AActor* OtherEntity)
{
	AssignLootFrom(OtherEntity);
}

bool UInventoryMainWidget::IsLootValid() const
{
	return bLootValid;
}

void UInventoryMainWidget::AssignLootFrom(AActor* OtherEntity)
{
	bLootValid = false;

	TArray<UBaseInventoryComponent*> Inventories;
	OtherEntity->GetComponents<>(UBaseInventoryComponent::StaticClass(), Inventories, false);

	if (IsValidForInit(Inventories))
	{
		bool bHasLootComponent = false;
		for (auto Inven : Inventories)
		{
			if (Inven->IsA<ULootInventoryComponent>())
			{
				if (!ensureAlwaysMsgf(IsValidForInit(Inventories), TEXT("Cannot have other inventories with ULootInventoryComponent")))
					return;

				InventoryComp_Loot = Cast<UPlayerEquipmentComponent>(Inven);
				bHasLootComponent = true;
				continue;
			}

			if (Inven->IsA<UDBEquipmentComponent>())
			{
				if (ensureAlwaysMsgf(!EquipmentComp_Loot, TEXT("Cannot have duplicate Equipment inventories")))
				{
					if (!ensureAlwaysMsgf(!bHasLootComponent, TEXT("Cannot have ULootInventoryComponent with DBEquipmentComponent")))
						return;

					EquipmentComp_Loot = Cast<UDBEquipmentComponent>(Inven);
				}

				else return;
			}
			else if (Inven->IsA<UPlayerEquipmentComponent>())
			{
				if (ensureAlwaysMsgf(!InventoryComp_Loot, TEXT("Cannot have duplicate PlayerEquipment inventories")))
					InventoryComp_Loot = Cast<UPlayerEquipmentComponent>(Inven);

				else return;
			}
		}
	}
	else return;

	bLootValid = true;
}

bool UInventoryMainWidget::IsValidForInit(const TArray<UBaseInventoryComponent*>& Inventories) const
{
	return IsValidNum(Inventories) && IsReadyForAssignment();
}


bool UInventoryMainWidget::IsValidNum(const TArray<UBaseInventoryComponent*>& Inventories) const
{
	if (Inventories.Num() > 2 || Inventories.Num() == 0)
		return false;

	return true;
}

bool UInventoryMainWidget::IsReadyForAssignment() const
{
	return EquipmentComp_Loot == nullptr && InventoryComp_Loot == nullptr;
}

void UInventoryMainWidget::ClearLoot()
{
	EquipmentComp_Loot = nullptr;
	InventoryComp_Loot = nullptr;

	if (InventoryLoot_Other)
		InventoryLoot_Other->Reset();
	if (InventoryLoot_Player)
		InventoryLoot_Player->Reset();
	if(EquipmentLoot_Weapon)
		EquipmentLoot_Weapon->Reset();
	if(EquipmentLoot_Consumable)
		EquipmentLoot_Weapon->Reset();
}

void UInventoryMainWidget::DisplayPlayerLoot(UPlayerEquipmentComponent* _InventoryComp, UDBEquipmentComponent* _EquipmentComp)
{
	InventoryLoot_Player->StartInit(_InventoryComp);
	EquipmentLoot_Weapon->StartInit(_EquipmentComp, ESlotType::WEAPON);
	EquipmentLoot_Consumable->StartInit(_EquipmentComp, ESlotType::CONSUMABLE);

	InventoryLoot_Player->SetVisibility(ESlateVisibility::Visible);
	EquipmentLoot_Weapon->SetVisibility(ESlateVisibility::Visible);
	EquipmentLoot_Consumable->SetVisibility(ESlateVisibility::Visible);

	InventoryLoot_Other->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryMainWidget::DisplayOtherLoot(UPlayerEquipmentComponent* _InventoryComp)
{
	InventoryLoot_Other->StartInit(_InventoryComp);

	InventoryLoot_Other->SetVisibility(ESlateVisibility::Visible);

	InventoryLoot_Player->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentLoot_Weapon->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentLoot_Consumable->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryMainWidget::HideLoots()
{
	InventoryLoot_Other->SetVisibility(ESlateVisibility::Collapsed);

	InventoryLoot_Player->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentLoot_Weapon->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentLoot_Consumable->SetVisibility(ESlateVisibility::Collapsed);
}