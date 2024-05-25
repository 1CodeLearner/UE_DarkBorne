// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuWidget.generated.h"

/**
 *
 */
class UButton;
class UDBGameInstance;
class UTextBlock;

UCLASS()
class DARKBORNE_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_CreateSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_JoinSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_ExitGame;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUserWidget> WBP_SessionUI;

	UFUNCTION()
	void OnCreatePressed();
	UFUNCTION()
	void OnJoinPressed();
	UFUNCTION()
	void OnExitPressed();

	void OnFindComplete(bool bWasSuccessful, bool bCanJoinSession);
	void OnJoinSessionEvent(bool bWasSuccessful);
	void OnCreateSessionComplete(bool bWasSuccessful);
	
	TObjectPtr<UDBGameInstance> GI;
	
};
