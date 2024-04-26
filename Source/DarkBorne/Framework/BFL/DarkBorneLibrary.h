// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DarkBorneLibrary.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API UDarkBorneLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static float CalculateDamage(AActor* Instigated, const TArray<FItem>& Items);

	//returns true if damage was applied
	UFUNCTION(BlueprintCallable)
	static bool ApplyDamage(AActor* Received, AActor* Instigated);

	//returns true if damage was applied
	UFUNCTION(BlueprintCallable)
	static bool ApplyDamageAmount(AActor* Received, float DamageAmount);
};
