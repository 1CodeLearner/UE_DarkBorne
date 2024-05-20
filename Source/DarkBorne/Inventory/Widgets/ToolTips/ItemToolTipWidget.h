// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTipWidget.generated.h"

/**
 *
 */

class UVerticalBox;
class UTextBlock;
class UItemDescriptionWidget;
class UItemObject;

UCLASS()
class DARKBORNE_API UItemToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void StartInit(UItemObject* ItemObject);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDescriptionWidget> ItemDescriptionWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_BaseStats;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Enchantments;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Effect;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_Category;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Lore;
};
