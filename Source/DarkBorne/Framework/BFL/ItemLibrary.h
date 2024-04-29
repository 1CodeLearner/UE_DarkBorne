// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../../Items/DBItem.h"
#include "../../ItemTypes/ItemType.h"
#include "ItemLibrary.generated.h"

/**
 *
 */
 class UItemObject; 

UCLASS()
class DARKBORNE_API UItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static int32 GetSlotIndexByObject(UItemObject* Item);
	UFUNCTION(BlueprintPure)
	static int32 GetSlotIndexByEnum(ESlotType SlotType);

};
