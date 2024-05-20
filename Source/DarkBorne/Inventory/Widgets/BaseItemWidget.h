// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseItemWidget.generated.h"

/**
 *
 */
class UItemObject;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemovedDelegate, UItemObject*, ItemObjectRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FClickedDelegate, UBaseItemWidget*, ItemWidgetClicked, bool, bIsRightClick);


UCLASS()
class DARKBORNE_API UBaseItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UItemObject* GetItemObject() const;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FRemovedDelegate OnRemoved;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FClickedDelegate OnClicked;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn))
	TObjectPtr<UItemObject> ItemObject;
};
