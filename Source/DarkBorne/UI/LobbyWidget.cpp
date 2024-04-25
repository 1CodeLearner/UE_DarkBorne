// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/TextBlock.h"

void ULobbyWidget::DisplayMessage(FString message)
{
	FText text = FText::FromString(message);
	Text_Message->SetText(text);
}
