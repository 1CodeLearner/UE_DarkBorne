// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItemWidget.h"
#include "../ItemObject.h"
#include "ToolTips/ItemToolTipWidget.h"
#include "Components/Border.h"

void UBaseItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(ItemToolTipWidgetClass)
	{
		ItemToolTipWidget = CreateWidget<UItemToolTipWidget>(GetOwningPlayer(), ItemToolTipWidgetClass);
	}

	if (ItemObject) 
	{
		ItemToolTipWidget->StartInit(ItemObject);
	}

	if (BackgroundBorder) 
	{
		BackgroundBorder->SetToolTip(ItemToolTipWidget);
	}
}

UItemObject* UBaseItemWidget::GetItemObject() const
{
	return ItemObject;
}
