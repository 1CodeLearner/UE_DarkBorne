// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemToolTipWidget.h"
#include "../../ItemObject.h"
#include "ItemDescriptionWidget.h"
#include "ToolTipSeparatorWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UItemToolTipWidget::StartInit(UItemObject* ItemObject)
{

	if (ensure(ItemDescriptionWidgetClass) && ensure(ItemBaseStatsWidgetClass) && ensure(ToolTipSeparatorWidgetClass) && ensure(ToolTipTitleWidgetClass) && ItemObject)
	{
		auto TitleWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ToolTipTitleWidgetClass);
		if (TitleWidget)
		{
			TitleWidget->SetText(ItemObject->GetDisplayName());
			VerticalBox_ToolTip->AddChild(TitleWidget);
		}

		//Change Later to TArray<FText>
		FText BaseStatText = ItemObject->GetBaseStatsText();
		if (!BaseStatText.IsEmpty())
		{
			auto BaseStatWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
			if (BaseStatWidget)
			{
				BaseStatWidget->SetText(BaseStatText);
				VerticalBox_ToolTip->AddChild(BaseStatWidget);
			}

			TArray<FText> EnchantmentsTexts = ItemObject->GetEnchantmentsTexts();
			if (!EnchantmentsTexts.IsEmpty())
			{
				for (int i = 0; i < EnchantmentsTexts.Num(); ++i)
				{
					auto EnchantmentWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
					if (EnchantmentWidget)
					{
						EnchantmentWidget->SetText(EnchantmentsTexts[0]);
						VerticalBox_ToolTip->AddChild(EnchantmentWidget);
					}
				}
			}
		}

		if (ItemObject->GetSlotType() == ESlotType::WEAPON)
		{
			FText EffectText = ItemObject->GetEffectText();
			if (!EffectText.IsEmpty())
			{
				auto EffectWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
				if (EffectWidget)
				{
					EffectWidget->SetText(EffectText);
					VerticalBox_ToolTip->AddChild(EffectWidget);
				}
			}

			CreateSeparator();
		}
		else {
			CreateSeparator();
			FText EffectText = ItemObject->GetEffectText();
			if (!EffectText.IsEmpty())
			{
				auto EffectWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
				if (EffectWidget)
				{
					EffectWidget->SetText(EffectText);
					VerticalBox_ToolTip->AddChild(EffectWidget);
				}
			}
			CreateSeparator();
		}

		//Change Later to TArray<FText>
		FText CategoryText = ItemObject->GetCategoryText();
		if (!CategoryText.IsEmpty())
		{
			auto CategoryWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
			if (CategoryWidget)
			{
				CategoryWidget->SetText(CategoryText);
				VerticalBox_ToolTip->AddChild(CategoryWidget);
			}
		}

		CreateSeparator();

		auto LoreWidget = CreateWidget<UItemDescriptionWidget>(GetOwningPlayer(), ItemDescriptionWidgetClass);
		if (LoreWidget)
		{
			LoreWidget->SetText(ItemObject->GetLoreText());
			VerticalBox_ToolTip->AddChild(LoreWidget);
		}
	}

	//VerticalBox_ToolTip->

	/*for(auto Child : VerticalBox_ToolTip->GetAllChildren())
	{
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Child);
		CanvasSlot->SetAutoSize(true);
	}*/
}

void UItemToolTipWidget::CreateSeparator()
{
	auto SeparatorWidget = CreateWidget<UToolTipSeparatorWidget>(GetOwningPlayer(), ToolTipSeparatorWidgetClass);
	if (SeparatorWidget)
	{
		VerticalBox_ToolTip->AddChild(SeparatorWidget);
	}
}
