// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

/**
 *
 */
 class UPlayerEquipmentComponent; 
 class UInventoryGridWidget; 

UCLASS()
class DARKBORNE_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryGrid_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerEquipmentComponent* PlayerEquipmentComp;
};
