// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootDisplayWidget.generated.h"

/**
 *
 */

class UPlayerEquipmentComponent;
class UDBEquipmentComponent;

class UInventoryGridWidget;
class UWidgetSwitcher;
UCLASS()
class DARKBORNE_API ULootDisplayWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void StartInit(UPlayerEquipmentComponent* InventoryComp, UDBEquipmentComponent* EquipmentComp);
	void Reset();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UPlayerEquipmentComponent> OtherInventoryComp;
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UDBEquipmentComponent> OtherEquipmentComp;


	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Player;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Loot;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Other;
};
