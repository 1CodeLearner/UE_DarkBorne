// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

public:
	void UpdateHeathBar(float currHP, float maxHP);
};
