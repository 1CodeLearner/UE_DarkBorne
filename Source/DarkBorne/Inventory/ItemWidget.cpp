// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"




bool UItemWidget::Initialize()
{
	return Super::Initialize();

}

void UItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	return Super::NativeOnDragDetected(InGeometry, InMouseEvent,OutOperation);
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseEnter(InGeometry,InMouseEvent);
}

void UItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseLeave(InMouseEvent);
}

void UItemWidget::Refresh()
{

}
