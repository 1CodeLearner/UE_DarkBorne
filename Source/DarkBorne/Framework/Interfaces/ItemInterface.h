// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../ItemTypes/ItemType.h"
#include "ItemInterface.generated.h"

class ADBItem;


// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */

DECLARE_DELEGATE_TwoParams(FInteractTimeUpdateDelegate, float /*CurrentTime*/, float /*MaxTime*/);
DECLARE_DELEGATE(FInteractFinishedDelegate);


class DARKBORNE_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FInteractTimeUpdateDelegate OnInteractTimeUpdate;
	FInteractFinishedDelegate OnInteractFinished;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//FDarkBorneStats GetDBStats() const;

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//ESlotType GetSlotType() const; 

	////figure out why this causes compile error
	////UFUNCTION(BlueprintNativeEvent)
	////FRarity GetRarity() const;

	//UFUNCTION(BlueprintNativeEvent)
	//void AssignStats(const FItem& _Item);
};
