// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWidget.h"
#include "Components/TextBlock.h"


void UItemDescriptionWidget::SetText(FString String)
{
	TextBlock_ItemDescription->SetText(FText::FromString(String));
}

void UItemDescriptionWidget::SetText(FText Text)
{
	TextBlock_ItemDescription->SetText(Text);
}
