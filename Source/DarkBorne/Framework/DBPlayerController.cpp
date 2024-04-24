// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerController.h"
#include "../UI/GameEndWidget.h"

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
