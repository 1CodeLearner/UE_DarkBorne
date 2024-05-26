// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/ProgressBar.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>

void UDBPlayerWidget::NativeConstruct()
{

}

void UDBPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (isShowDamageUI)
	{
		// 데미지 UI 를 서서히 보이지 않게 하자.	
		// 1. currOpacity 를 줄어들게 하자
		currOpacity -= InDeltaTime;
		// 2. 그 값을 데미지 UI 의 opacity 값으로 설정
		DamageUI->SetRenderOpacity(currOpacity);
		// 3. currOpacity 가 0보다 같거나 작아지면
		if (currOpacity <= 0)
		{
			// 4. 데미지 UI 를 안보이게 하자
			isShowDamageUI = false;
		}
	}
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

void UDBPlayerWidget::UpdateQBorder(float Q_CurrCoolTime, float Q_MaxCoolTime)
{
	if (Q_CurrCoolTime >= Q_MaxCoolTime)
	{
		Q_Border->SetBrushFromMaterial(MI_Wedge);
	}
	else
	{
		Q_Border->SetBrushFromTexture(BlankImage);
	}

	
}

void UDBPlayerWidget::UpdateQBorder_Active(bool isVanish)
{
	if (isVanish)
	{
		Q_Border_Active->SetBrushFromMaterial(MI_Wedge_Active);

		Q_Border->SetBrushFromTexture(BlankImage);
	}
	else
	{
		Q_Border_Active->SetBrushFromTexture(BlankImage);
	}
}

void UDBPlayerWidget::UpdateEBorder(float E_CurrCoolTime, float E_MaxCoolTime)
{
	if (E_CurrCoolTime >= E_MaxCoolTime)
	{
		E_Border->SetBrushFromMaterial(MI_Wedge);
	}
	else
	{
		E_Border->SetBrushFromTexture(BlankImage);
	}
}

void UDBPlayerWidget::UpdateEBorder_Active(bool isSpawnKnife)
{
	if (isSpawnKnife)
	{
		E_Border_Active->SetBrushFromMaterial(MI_Wedge_Active);

		E_Border->SetBrushFromTexture(BlankImage);
	}
	else
	{
		E_Border_Active->SetBrushFromTexture(BlankImage);
	}
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

void UDBPlayerWidget::ShowDamageUI()
{
	isShowDamageUI = true;
	currOpacity = 1;
	PlayAnimation(DamageAnim);
}
