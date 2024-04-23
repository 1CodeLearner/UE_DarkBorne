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
class ULootInventoryComponent;
class ULootEquipmentComponent;

enum class EEntityType : uint8;

UCLASS()
class DARKBORNE_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Dynamically change inventory and equipment inventory size
	UFUNCTION(BlueprintImplementableEvent)
	void StartInit(EEntityType EntityType);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryGrid_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UPlayerEquipmentComponent* PlayerEquipmentComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	ULootInventoryComponent* LootInventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	ULootEquipmentComponent* LootEquipmentComp;
};
