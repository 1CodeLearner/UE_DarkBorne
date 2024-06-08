// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGridWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UInventoryGridWidget::StartInit(UPlayerEquipmentComponent* InventoryComp/*, EGridWidgetType _GridWidgetType*/)
{
	InventoryComponent = InventoryComp;
	TileSize = UPlayerEquipmentComponent::GetTileSize(InventoryComp);

	UCanvasPanelSlot* CanvasPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridBorder);

	CanvasPanel->SetSize(InventoryComponent->GetSize());

	CreateLineSegments();
	Refresh();

	auto nice = Cast<UBaseInventoryComponent>(InventoryComponent);
	if (nice)
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryGridWidget::Refresh);

	//GridWidgetType = _GridWidgetType;
}

void UInventoryGridWidget::Reset()
{
	if (InventoryComponent && InventoryComponent->OnInventoryChanged.IsBound())
	{
		InventoryComponent->OnInventoryChanged.Clear();
	}
	InventoryComponent = nullptr;
}

void UInventoryGridWidget::CreateLineSegments()
{
	Lines.Empty();

	for (int32 i = 0; i < InventoryComponent->GetColumn() + 1; ++i)
	{
		int32 x = i * TileSize;
		FVector2D Start(x, 0);
		FVector2D End(x, InventoryComponent->GetRow() * TileSize);

		Lines.Add(FLine(Start, End));
	}

	for (int32 i = 0; i < InventoryComponent->GetRow() + 1; ++i)
	{
		int32 y = i * TileSize;
		FVector2D Start(0, y);
		FVector2D End(InventoryComponent->GetColumn() * TileSize, y);

		Lines.Add(FLine(Start, End));
	}
}