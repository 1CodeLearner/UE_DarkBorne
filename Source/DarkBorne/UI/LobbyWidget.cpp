// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"

void ULobbyWidget::EnableMessage(bool bEnabled)
{
	if (bEnabled)
	{
		Text_Message->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_Message->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULobbyWidget::EnableTimer(bool bEnabled)
{
	if (bEnabled)
	{
		Text_Timer->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_Timer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULobbyWidget::DisplayMessage(FString message)
{
	FText text = FText::FromString(message);
	Text_Message->SetText(text);
}

void ULobbyWidget::DisplayTimer(float Time)
{
	FText TimeText = FText::FromString(FString::Printf(TEXT("Time remaining: %.0f"), Time));
	Text_Timer->SetText(TimeText);
}
