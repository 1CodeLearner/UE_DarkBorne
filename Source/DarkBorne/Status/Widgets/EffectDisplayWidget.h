// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EffectDisplayWidget.generated.h"

/**
 *
 */
class UEffectIconWidget;
class UHorizontalBox;
class UDBEffect;
UCLASS()
class DARKBORNE_API UEffectDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void RemoveEffectIcon(UEffectIconWidget* IconWidget);

protected:
	virtual void NativeOnInitialized() override;


protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UHorizontalBox> HoriBox_EffectIcons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UEffectIconWidget> EffectIconWidget;

	//void OnEffectStart(UDBEffect* effect, UMaterialInterface* IconDisplay);
	void OnEffectUpdate(TArray<UDBEffect*> UpdatedEffects);
		

};
