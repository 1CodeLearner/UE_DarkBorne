// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

class UTextBlock;
class ACharacter;
class UCanvasPanel;
class UProgressBar;
enum class EInteractState : uint8;
/**
 *
 */
UCLASS()
class DARKBORNE_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	void DisplayBeginTrace(bool bDisplay, AActor* ActorFound = nullptr);
	void DisplayBeginInteract(bool bDisplay);
protected:
	virtual void NativeOnInitialized() override;

	void SetNameText(FString NameStr);
	void SetActionText(FString ActionStr);

	void OnInteractActorUpdate(AActor* ActorFound, EInteractState InteractState);
	void OnInteractTimeUpdate(float CurrentTime, float MaxTime);
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Name;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Text_Action;
	
	UPROPERTY(meta=(BindWidget))
	UProgressBar* ProgressBar_Interact;

	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* Canvas_BeginTrace;
	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* Canvas_BeginInteract;
};
