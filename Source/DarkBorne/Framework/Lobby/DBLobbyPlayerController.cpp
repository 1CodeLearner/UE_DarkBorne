// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyPlayerController.h"
#include "../../UI/LobbyWidget.h"

void ADBLobbyPlayerController::Client_DisplayMessage_Implementation(const FString& msg)
{
	if (ensureAlways(LobbyWidgetClass) && !LobbyWidget)
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		LobbyWidget->AddToViewport();
	}

	if (LobbyWidget)
	{
		LobbyWidget->DisplayMessage(msg);
	}
}
