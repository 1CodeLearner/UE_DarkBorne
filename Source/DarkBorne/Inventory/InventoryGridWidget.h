// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BaseGridWidget.h"
#include "PlayerEquipmentComponent.h"
#include "InventoryGridWidget.generated.h"

/**
 * 
 */

 class UCanvasPanel;
 class UBorder;
UCLASS()
class DARKBORNE_API UInventoryGridWidget : public UBaseGridWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StartInit(UPlayerEquipmentComponent* InventoryComp/*, EGridWidgetType _GridWidgetType*/);

	void Reset();
		
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerEquipmentComponent> InventoryComponent;

protected:

	UPROPERTY(BlueprintReadOnly)
	TArray<FLine> Lines;

	UPROPERTY(BlueprintReadOnly)
	float TileSize;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

private:
	void CreateLineSegments();
};
