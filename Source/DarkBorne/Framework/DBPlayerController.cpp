// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerController.h"
#include "../UI/GameEndWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>

void ADBPlayerController::Client_DisplayGameResult_Implementation(bool bHasWon)
{
	if (ensureAlways(GameEndWidgetClass) && !GameEndWidget)
	{
		GameEndWidget = CreateWidget<UGameEndWidget>(this, GameEndWidgetClass);
	}

	if (GameEndWidget)
	{
		if (bHasWon) {
			GameEndWidget->SetGameStateText(FText::FromString(FString::Printf(TEXT("WON"))));
		}
		else
			GameEndWidget->SetGameStateText(FText::FromString(FString::Printf(TEXT("LOST"))));

		GameEndWidget->AddToViewport();
	}
}

void ADBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController()) 
	{
		UE_LOG(LogTemp,Warning, TEXT("YEs %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NOOE"));
	}
}
