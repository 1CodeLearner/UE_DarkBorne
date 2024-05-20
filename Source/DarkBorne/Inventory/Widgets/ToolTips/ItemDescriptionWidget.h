// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDescriptionWidget.generated.h"

/**
 *
 */

class UTextBlock;

UCLASS()
class DARKBORNE_API UItemDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetText(FString String);
	void SetText(FText Text);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ItemDescription;
};
