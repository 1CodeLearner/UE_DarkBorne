// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>

void UDBPlayerWidget::NativeConstruct()
{

}

void UDBPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}


void UDBPlayerWidget::UpdateHeathBar(float currHP, float maxHP)
{
	float percent = currHP / maxHP;
	HealthBar->SetPercent(percent);
}
