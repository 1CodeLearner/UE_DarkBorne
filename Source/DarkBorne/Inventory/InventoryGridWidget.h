// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEquipmentComponent.h"
#include "InventoryGridWidget.generated.h"

/**
 * 
 */

 class UCanvasPanel;
 class UBorder;
UCLASS()
class DARKBORNE_API UInventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StartInit(UPlayerEquipmentComponent* InventoryComp);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPlayerEquipmentComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FLine> Lines;

	UPROPERTY(BlueprintReadOnly)
	float TileSize;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;
private:
	void CreateLineSegments();
};
