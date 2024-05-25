// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ACharacter;
class UDBInteractionComponent;

USTRUCT()
struct FDisplayInfo
{
	GENERATED_BODY();
	FDisplayInfo() = default;
	FDisplayInfo(FString _Action, FString _Name)
	{
		Action = _Action;
		Name = _Name;
	}
	UPROPERTY()
	FString Action;
	UPROPERTY()
	FString Name;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class DARKBORNE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IInteractionInterface();
	float GetInteractionTime() const;
	void SetInteractionTime(float InteractTime);

	virtual void BeginInteract(UDBInteractionComponent* InteractionComp) = 0;
	virtual void ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character) = 0;
	virtual void InterruptInteract() = 0;

	virtual void BeginTrace() = 0;
	virtual void EndTrace() = 0;

	virtual bool CanInteract() const = 0;
	virtual void SetCanInteract(bool bAllowInteract) = 0;

	virtual FDisplayInfo GetDisplayInfo() const = 0;
private:
	float InteractionTime;
};
