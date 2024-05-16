// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BaseGridWidget.h"
#include "EquipmentGridWidget.generated.h"

/**
 * 
 */
 class UDBEquipmentComponent;
 class UBorder;

UCLASS()
class DARKBORNE_API UEquipmentGridWidget : public UBaseGridWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StartInit(UDBEquipmentComponent* EquipmentComp, ESlotType _SlotType/*, EGridWidgetType _GridWidgetType*/);

	void Reset();

protected:		
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDBEquipmentComponent> EquipmentComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDBEquipmentComponent> TaxiToServer;
	
	UPROPERTY(BlueprintReadOnly)
	ESlotType SlotType;

	UPROPERTY(BlueprintReadOnly)
	float TileSize;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UBorder> GridBorder;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bIsBeingHeld;
};
