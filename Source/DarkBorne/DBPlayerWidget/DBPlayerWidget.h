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

	UPROPERTY(meta = (BindWidget))
	class UBorder* Q_Border;
	UPROPERTY(meta = (BindWidget))
	class UBorder* E_Border;

	UPROPERTY(meta = (BindWidget))
	class UBorder* Q_Border_Active;
	UPROPERTY(meta = (BindWidget))
	class UBorder* E_Border_Active;
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponSlot;

	UPROPERTY(meta = (BindWidget))
	class UImage* ConsumeSlot;

	UPROPERTY(EditAnywhere)
	class UTexture2D* BlankImage;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MI_Wedge;
	UPROPERTY(EditAnywhere)
	class UMaterialInstance* MI_Wedge_Active;
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* DamageUI;
	float currOpacity = 0;
	bool isShowDamageUI = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* DamageAnim;

public:
	void UpdateHeathBar(float currHP, float maxHP);
	void UpdateQSkillBar(float Q_CurrCoolTime, float Q_MaxCoolTime);
	void UpdateESkillBar(float E_CurrCoolTime, float E_MaxCoolTime);
	void UpdateSlot(TArray<UItemObject*> EquipSlotArray);

	void UpdateQBorder(float Q_CurrCoolTime, float Q_MaxCoolTime);
	void UpdateQBorder_Active(bool isVanish);
	void UpdateEBorder(float E_CurrCoolTime, float E_MaxCoolTime);
	void UpdateEBorder_Active(bool isSpawnKnife);

private:
	void UpdateWeaponSlot(TArray<UItemObject*> EquipSlotArray);
	void UpdateConsumeSlot(TArray<UItemObject*> EquipSlotArray);

public:
	void ShowDamageUI();
};
