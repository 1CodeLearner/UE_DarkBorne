// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/ItemObject.h"
#include "DBPlayerWidget.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UDBPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Q_SkillBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* E_SkillBar;

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* ConsumeSlot;
public:
	void UpdateHeathBar(float currHP, float maxHP);
	void UpdateQSkillBar(float Q_CurrCoolTime, float Q_MaxCoolTime);
	void UpdateESkillBar(float E_CurrCoolTime, float E_MaxCoolTime);
	void UpdateSlot(TArray<UItemObject*> EquipSlotArray);

private:
	void UpdateWeaponSlot(TArray<UItemObject*> EquipSlotArray);
	void UpdateConsumeSlot(TArray<UItemObject*> EquipSlotArray);
};
