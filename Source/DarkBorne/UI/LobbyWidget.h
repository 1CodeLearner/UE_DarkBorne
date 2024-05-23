// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 *
 */
class UTextBlock;
UCLASS()
class DARKBORNE_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void DisplayMessage(FString message);
	void DisplayTimer(float Time);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Message;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Timer;
};
