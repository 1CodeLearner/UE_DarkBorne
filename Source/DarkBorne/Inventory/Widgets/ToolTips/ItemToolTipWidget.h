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
class UToolTipSeparatorWidget;
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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDescriptionWidget> ItemBaseStatsWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDescriptionWidget> ToolTipTitleWidgetClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UToolTipSeparatorWidget> ToolTipSeparatorWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox_ToolTip;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> MI_Common;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> MI_Rare;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> MI_Epic;

private:
	void CreateSeparator();
};
