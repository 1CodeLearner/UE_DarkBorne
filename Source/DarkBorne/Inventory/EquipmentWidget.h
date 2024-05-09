// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
class UEquipmentGridWidget;
UCLASS()
class DARKBORNE_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Settings")
	TArray<UEquipmentGridWidget*> EquipmentGridWidgets;
};
