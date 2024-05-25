// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionUIWidget.generated.h"

/**
 *
 */

USTRUCT()
struct FSessionInfo
{
	GENERATED_BODY()

	int PlayerCount;
	float CountdownTime;
};

class USpinBox;
class UComboBoxString;
class UButton;

UCLASS()
class DARKBORNE_API USessionUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FSessionInfo GetSessionInfo() const;
	void BindWithCloseWindow(const FScriptDelegate& CloseButtonDelegate);
	void BindWithCreateSession(const FScriptDelegate& CreateSessionDelegate);
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USpinBox> SpinBox_PlayerCount;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ComboBox_CountdownTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_CreateSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button_CloseWindow;

};
