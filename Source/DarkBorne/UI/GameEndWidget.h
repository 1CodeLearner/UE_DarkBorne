// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndWidget.generated.h"

/**
 *
 */
class UTextBlock;
UCLASS()
class DARKBORNE_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGameStateText(FText text);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameStateText;
};
