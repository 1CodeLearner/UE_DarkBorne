// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UCharacterAnimUtilityLibrary.generated.h"

/**
 * 
 */
 UENUM(BlueprintType)
enum class EAnimationFlags : uint8
{
    ANIM_NONE       = 0,
    ANIM_IS_HIT     = 1 << 0,
	ANIM_IS_DEAD    = 1 << 1,
    // 추가 상태들...
};

UCLASS()
class DARKBORNE_API UUCharacterAnimUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="Animation|Utility")
    static void SetAnimationStatus(uint8& Status, EAnimationFlags Flag);

    UFUNCTION(BlueprintCallable, Category="Animation|Utility")
    static void ClearAnimationStatus(uint8& Status, EAnimationFlags Flag);

    UFUNCTION(BlueprintPure, Category="Animation|Utility")
    static bool IsAnimationStatusSet(uint8 Status, EAnimationFlags Flag);

};
