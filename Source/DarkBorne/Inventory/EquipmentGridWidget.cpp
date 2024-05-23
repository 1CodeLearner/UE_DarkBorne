// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentGridWidget.h"
#include "DBEquipmentComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "InventoryGridWidget.h"

void UEquipmentGridWidget::StartInit(UDBEquipmentComponent* EquipmentComp, ESlotType _SlotType/*, EGridWidgetType _GridWidgetType*/)
{
	bIsBeingHeld = false;
	EquipmentComponent = EquipmentComp;
	TileSize = EquipmentComponent->GetTileSize();
	SlotType = _SlotType;

	Refresh();

	EquipmentComponent->OnInventoryChanged.AddDynamic(this, &UEquipmentGridWidget::Refresh);

	//GridWidgetType = _GridWidgetType;
}

void UEquipmentGridWidget::Reset()
{
	if (EquipmentComponent && EquipmentComponent->OnInventoryChanged.IsBound())
	{
		EquipmentComponent->OnInventoryChanged.Clear();
	}
	EquipmentComponent = nullptr;
}
