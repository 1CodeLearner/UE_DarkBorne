// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ItemTypes/ItemType.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize() override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void Refresh();

	
public:
	UPROPERTY(meta=(BindWidget))
	class USizeBox* BackgroundSizeBox;

	UPROPERTY(meta=(BindWidget))
	class UBorder* BackgroundBorder;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* ItemImage;
	
	UPROPERTY(meta=(BindWidget))
	class UImage* ItemOverImage;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TileSize  = 0;
	
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//class FItem ItemData;
		
};
