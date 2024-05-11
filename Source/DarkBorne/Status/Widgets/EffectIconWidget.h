// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EffectIconWidget.generated.h"

/**
 *
 */

class UDBEffect;
class UMaterialInterface;
class UImage;
class UEffectDisplayWidget;
class UProgressBar;

UCLASS()
class DARKBORNE_API UEffectIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartInit(UEffectDisplayWidget* ParentWidget, UDBEffect* Effect, UMaterialInterface* Icon);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_EffectIcon;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_TimeRemaining;

protected:
	void OnEveryTick(float TotalTime, float RemainingTime);
	//void OnStop();

private:
	TObjectPtr<UEffectDisplayWidget> EffectDisplayWidget;
};
