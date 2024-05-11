// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectIconWidget.h"
#include "../DBEffect.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "EffectDisplayWidget.h"

void UEffectIconWidget::StartInit(UEffectDisplayWidget* ParentWidget, UDBEffect* Effect, UMaterialInterface* Icon)
{
	EffectDisplayWidget = ParentWidget;

	Effect->OnEveryTick.BindUObject(this, &UEffectIconWidget::OnEveryTick);
	Effect->OnStop.BindUObject(this, &UEffectIconWidget::OnStop);

	Image_EffectIcon->SetBrushFromMaterial(Icon);
}

void UEffectIconWidget::OnEveryTick(float TotalTime, float RemainingTime)
{
	float Remaining = TotalTime - RemainingTime;
	ProgressBar_TimeRemaining->SetPercent(Remaining / TotalTime);
}

void UEffectIconWidget::OnStop()
{
	EffectDisplayWidget->RemoveEffectIcon(this);
}
