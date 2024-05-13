// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentGridWidget.h"
#include "DBEquipmentComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InventoryGridWidget.h"

void UEquipmentGridWidget::StartInit(UDBEquipmentComponent* EquipmentComp, ESlotType _SlotType)
{
	bIsBeingHeld = false;
	EquipmentComponent = EquipmentComp;
	TileSize = EquipmentComponent->GetTileSize();
	SlotType = _SlotType;

	UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);

	CanvasPanel->SetSize(EquipmentComponent->GetSize());

	Refresh();

	EquipmentComponent->OnInventoryChanged.AddDynamic(this, &UEquipmentGridWidget::Refresh);

	auto Taxi = GetOwningPlayerPawn()->GetComponentByClass<UDBEquipmentComponent>();
	if (ensureAlways(Taxi))
	{
		TaxiToServer = Taxi;
	}

}

void UEquipmentGridWidget::Reset()
{
	if (EquipmentComponent && EquipmentComponent->OnInventoryChanged.IsBound())
	{
		EquipmentComponent->OnInventoryChanged.Clear();
	}
	EquipmentComponent = nullptr;
}
