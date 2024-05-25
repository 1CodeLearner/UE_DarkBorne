// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInterface.h"
#include "Components/Image.h"

void UItemDescriptionWidget::SetText(FString String)
{
	TextBlock_ItemDescription->SetText(FText::FromString(String));
}

void UItemDescriptionWidget::SetText(FText Text)
{
	TextBlock_ItemDescription->SetText(Text);
}

void UItemDescriptionWidget::SetImageMaterial(UMaterialInterface* MI_Rarity)
{
	if(Image_Rarity && MI_Rarity)
	{
		 Image_Rarity->SetBrushFromMaterial(MI_Rarity);
	}
}
