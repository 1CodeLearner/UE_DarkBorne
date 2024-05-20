// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItemWidget.h"
#include "../ItemObject.h"

UItemObject* UBaseItemWidget::GetItemObject() const
{
	return ItemObject;
}
