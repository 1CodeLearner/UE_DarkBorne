// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "../Framework/DBGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

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
}

void UMainMenuWidget::OnCreatePressed()
{
	if (GI)
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Creating new session..."))));
		GI->CreateMySession();
		Button_CreateSession->SetIsEnabled(false);
		Button_JoinSession->SetIsEnabled(false);
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
	}
}

void UMainMenuWidget::OnExitPressed()
{
	UKismetSystemLibrary::QuitGame(GetOwningPlayer(), GetOwningPlayer(),EQuitPreference::Quit, true);
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
		}
	}
	else
	{
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Find session unsuccessful"))));
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
		Text_Message->SetText(FText::FromString(FString::Printf(TEXT("Create session unsuccessful"))));
	}
}
