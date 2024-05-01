// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;
class ACharacter;

/**
 *
 */
UCLASS()
class DARKBORNE_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	void DisplayInteract(FString NameStr, FString ActionStr);
	
protected:
	virtual void NativeOnInitialized() override;

	void SetNameText(FString NameStr);
	void SetActionText(FString ActionStr);

	void OnInteractActorUpdate(AActor* ActorFound);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Name;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Action;

};
