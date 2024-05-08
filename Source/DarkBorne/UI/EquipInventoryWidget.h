// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipInventoryWidget.generated.h"

/**
 *
 */
class UInventoryGridWidget; 
class UEquipmentWidget; 

UCLASS()
class DARKBORNE_API UEquipInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<UInventoryGridWidget> InventoryWidget;
	TObjectPtr<UEquipmentWidget> EquipmentWidget;
};
