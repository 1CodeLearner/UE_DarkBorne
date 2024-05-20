// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTipWidget.h"
#include "../../ItemObject.h"
#include "ItemDescriptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UItemToolTipWidget::StartInit(UItemObject* ItemObject)
{

	if (ensure(ItemDescriptionWidgetClass) && ItemObject)
	{
		TextBlock_Name->SetText(ItemObject->GetDisplayName());

		//Change Later to TArray<FText>
		FText BaseStatText = ItemObject->GetBaseStatsText();
		if (!BaseStatText.IsEmpty()) 
		{
			auto BaseStatWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
			if (BaseStatWidget)
			{
				BaseStatWidget->TextBlock_ItemDescription->SetText(BaseStatText);
				VerticalBox_BaseStats->AddChild(BaseStatWidget);
			}
		}

		TArray<FText> EnchantmentsTexts = ItemObject->GetEnchantmentsTexts();
		if (!EnchantmentsTexts.IsEmpty()) 
		{
			for (int i = 0; i < EnchantmentsTexts.Num(); ++i)
			{
				auto EnchantmentWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
				if (EnchantmentWidget)
				{

				}
			}
		}

		FText EffectText = ItemObject->GetEffectText();
		if (!EffectText.IsEmpty()) 
		{
			TextBlock_Effect->SetText(EffectText);
		}
		else {
			TextBlock_Effect->SetText(FText());
		}

		//Change Later to TArray<FText>
		FText CategoryText = ItemObject->GetCategoryText();
		if (!CategoryText.IsEmpty()) 
		{
			auto CategoryWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
			if (CategoryWidget)
			{
				CategoryWidget->TextBlock_ItemDescription->SetText(CategoryText);
				VerticalBox_Category->AddChild(CategoryWidget);
			}
		}

		TextBlock_Lore->SetText(ItemObject->GetLoreText());
	}
}
