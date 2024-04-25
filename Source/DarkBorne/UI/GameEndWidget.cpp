// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"
#include "Components/TextBlock.h"

void UGameEndWidget::SetGameStateText(FText text)
{
	GameStateText->SetText(text);
}
