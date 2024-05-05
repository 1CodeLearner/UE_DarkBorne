// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMainWidget.h"
#include "BaseInventoryComponent.h"
#include "PlayerEquipmentComponent.h"
#include "DBEquipmentComponent.h"
#include "LootInventoryComponent.h"

#include "LootDisplayWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventoryMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
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
	if(bEnabled)
	{
		if(IsLootValid())
		{
			WBP_LootDisplay->SetVisibility(ESlateVisibility::Visible);
			WBP_LootDisplay->StartInit(InventoryComp_Loot, EquipmentComp_Loot);
		}
		
		if(ensureAlways(!IsInViewport()))
		{
			AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
				GetOwningPlayer(), this, EMouseLockMode::LockOnCapture);
			GetOwningPlayer()->bShowMouseCursor = true;
		}
	}
	else
	{
		if(IsLootValid())
		{
			WBP_LootDisplay->SetVisibility(ESlateVisibility::Collapsed);	
			
			ClearLoot();
			WBP_LootDisplay->Reset();
			bLootValid = false;
		}
		
		if(ensureAlways(IsInViewport()))
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
}
