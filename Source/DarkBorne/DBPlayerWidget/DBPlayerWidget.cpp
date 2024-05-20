// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>

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

void UDBPlayerWidget::UpdateSlot(TArray<UItemObject*> EquipSlotArray)
{
	UpdateWeaponSlot(EquipSlotArray);
	UpdateConsumeSlot(EquipSlotArray);
}

void UDBPlayerWidget::UpdateWeaponSlot(TArray<UItemObject*> EquipSlotArray)
{
	if(EquipSlotArray[0] == nullptr)
	{
		WeaponSlot->SetBrushFromTexture(BlankImage);	
	}
	else
	{
		UTexture2D* WeaponImage = EquipSlotArray[0]->GetIconTexture();
		WeaponSlot->SetBrushFromTexture(WeaponImage);
	}

}

void UDBPlayerWidget::UpdateConsumeSlot(TArray<UItemObject*> EquipSlotArray)
{
	if (EquipSlotArray[7] == nullptr)
	{
		ConsumeSlot->SetBrushFromTexture(BlankImage);
	}
	else
	{
		UTexture2D* ConsumeImage = EquipSlotArray[7]->GetIconTexture();
		ConsumeSlot->SetBrushFromTexture(ConsumeImage);
	}
}
