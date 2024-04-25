// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyPlayerController.h"
#include "../../UI/LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>

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

void ADBLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("YEs %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NOOE"));
	}
}
