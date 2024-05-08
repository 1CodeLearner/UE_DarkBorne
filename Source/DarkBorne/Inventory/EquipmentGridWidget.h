// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentGridWidget.generated.h"

/**
 * 
 */
 class UDBEquipmentComponent;
 class UBorder;

UCLASS()
class DARKBORNE_API UEquipmentGridWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StartInit(UDBEquipmentComponent* EquipmentComp, ESlotType _SlotType);

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
