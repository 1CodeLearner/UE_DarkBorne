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
UCLASS()
class DARKBORNE_API UItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static float CalculateDamage(AActor* Instigated, const TArray<FItem>& Items);

	UFUNCTION(BlueprintCallable)	
	static float ApplyDamage(AActor* Received, AActor* Instigated);

};
