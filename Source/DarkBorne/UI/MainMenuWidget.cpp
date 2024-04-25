// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "../Framework/DBGameInstance.h"
#include "Components/TextBlock.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_StartGame->OnPressed.AddDynamic(this, &UMainMenuWidget::OnPressed);
	GI = GetWorld()->GetGameInstance<UDBGameInstance>();
	GI->onSearchComplete.BindUObject(this, &UMainMenuWidget::OnSearchComplete);
}

void UMainMenuWidget::OnPressed()
{
	if (GI) 
	{
		GI->FindOtherSession();
		Button_StartGame->SetIsEnabled(false);
	}
}

void UMainMenuWidget::OnSearchComplete(int32 TotalSessions)
{
	if (TotalSessions > 0) 
	{
		Text_StartGame->SetText(FText::FromString(FString::Printf(TEXT("Joining Session..."))));
	}
	else if ( TotalSessions == -1 )
	{	
		Text_StartGame->SetText(FText::FromString(FString::Printf(TEXT("Search Session Failed"))));
		Button_StartGame->SetIsEnabled(true);
	}
	else 
	{
		Text_StartGame->SetText(FText::FromString(FString::Printf(TEXT("Creating Session..."))));
	}
}
