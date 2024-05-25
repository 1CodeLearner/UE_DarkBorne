// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionUIWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/SpinBox.h"

FSessionInfo USessionUIWidget::GetSessionInfo() const
{
	int32 PlayerCount = SpinBox_PlayerCount->GetValue();
	float CountdownTime = FCString::Atof(*ComboBox_CountdownTime->GetSelectedOption());

	return FSessionInfo(PlayerCount, CountdownTime);
}

void USessionUIWidget::BindWithCloseWindow(const FScriptDelegate& CloseButtonDelegate)
{
	Button_CloseWindow->OnClicked.AddUnique(CloseButtonDelegate);
}

void USessionUIWidget::BindWithCreateSession(const FScriptDelegate& CreateSessionDelegate)
{
	Button_CreateSession->OnClicked.AddUnique(CreateSessionDelegate);
}
