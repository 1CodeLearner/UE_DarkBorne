// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "../Framework/DBGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SessionUIWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_CreateSession->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreatePressed);

	Button_JoinSession->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinPressed);

	Button_ExitGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitPressed);

	GI = GetWorld()->GetGameInstance<UDBGameInstance>();
	GI->OnFindComplete.BindUObject(this, &UMainMenuWidget::OnFindComplete);
	GI->OnCreateComplete.BindUObject(this, &UMainMenuWidget::OnCreateSessionComplete);
	GI->OnJoinSessionEvent.BindUObject(this, &UMainMenuWidget::OnJoinSessionEvent);
	if (WBP_SessionUI)
	{
		CloseButtonDelegate.BindUFunction(this, "OnCloseButtonPressed");
		WBP_SessionUI->BindWithCloseWindow(CloseButtonDelegate);

		CreateSessionDelegate.BindUFunction(this, "OnCreateSessionPressed");
		WBP_SessionUI->BindWithCreateSession(CreateSessionDelegate);
	}
	WBP_SessionUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::OnCloseButtonPressed()
{
	WBP_SessionUI->SetVisibility(ESlateVisibility::Collapsed);
	DisplayOptions();
}

void UMainMenuWidget::OnCreateSessionPressed()
{
	WBP_SessionUI->SetVisibility(ESlateVisibility::Collapsed);
	DisplayOptions();
	Button_CreateSession->SetIsEnabled(false);
	Button_JoinSession->SetIsEnabled(false);
	Button_ExitGame->SetIsEnabled(false);
	Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Creating new session..."))));
	
	FSessionInfo SessionInfo = WBP_SessionUI->GetSessionInfo();
	
	GI->CreateMySession(SessionInfo.PlayerCount, SessionInfo.CountdownTime);
}

void UMainMenuWidget::OnCreatePressed()
{
	if (GI)
	{
		WBP_SessionUI->SetVisibility(ESlateVisibility::Visible);
		HideOptions();
	}
}

void UMainMenuWidget::OnJoinPressed()
{
	if (GI)
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Finding session to join..."))));
		GI->FindOtherSession();
		Button_CreateSession->SetIsEnabled(false);
		Button_JoinSession->SetIsEnabled(false);
		Button_ExitGame->SetIsEnabled(false);
	}
}

void UMainMenuWidget::OnExitPressed()
{
	UKismetSystemLibrary::QuitGame(GetOwningPlayer(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::OnFindComplete(bool bWasSuccessful, bool bCanJoinSession)
{
	if (bWasSuccessful)
	{
		if (bCanJoinSession)
		{
			Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Find session successful"))));
		}
		else
		{
			Text_Message->SetText(FText::FromString(FString::Printf(TEXT("No available sessions to join"))));
			Button_CreateSession->SetIsEnabled(true);
			Button_JoinSession->SetIsEnabled(true);
			Button_ExitGame->SetIsEnabled(true);
		}
	}
	else
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Find session unsuccessful"))));
		Button_CreateSession->SetIsEnabled(true);
		Button_JoinSession->SetIsEnabled(true);
		Button_ExitGame->SetIsEnabled(true);
	}
}

void UMainMenuWidget::OnJoinSessionEvent(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Joining session..."))));
	}
	else
	{
		Button_CreateSession->SetIsEnabled(true);
		Button_JoinSession->SetIsEnabled(true);
		Button_ExitGame->SetIsEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("Join session Failed"));
	}
}

void UMainMenuWidget::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Create session successful, traveling as host..."))));
	}
	else {
		Button_CreateSession->SetIsEnabled(true);
		Button_JoinSession->SetIsEnabled(true);
		Button_ExitGame->SetIsEnabled(true);
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Create session unsuccessful"))));
	}
}

void UMainMenuWidget::DisplayOptions()
{
	Button_CreateSession->SetVisibility(ESlateVisibility::Visible);
	Button_JoinSession->SetVisibility(ESlateVisibility::Visible);
	Button_ExitGame->SetVisibility(ESlateVisibility::Visible);
	Text_Message->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::HideOptions()
{
	Button_CreateSession->SetVisibility(ESlateVisibility::Collapsed);
	Button_JoinSession->SetVisibility(ESlateVisibility::Collapsed);
	Button_ExitGame->SetVisibility(ESlateVisibility::Collapsed);
	Text_Message->SetVisibility(ESlateVisibility::Collapsed);
}
