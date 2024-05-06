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

void UDBPlayerWidget::UpdateQSkillBar(float Q_CurrCoolTime, float Q_MaxCoolTime)
{
	float percent = Q_CurrCoolTime / Q_MaxCoolTime;
	Q_SkillBar->SetPercent(percent);
}

void UDBPlayerWidget::UpdateESkillBar(float E_CurrCoolTime, float E_MaxCoolTime)
{
	float percent = E_CurrCoolTime / E_MaxCoolTime;
	E_SkillBar->SetPercent(percent);
}
