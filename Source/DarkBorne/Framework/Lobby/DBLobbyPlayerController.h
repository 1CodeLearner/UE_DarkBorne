// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DBLobbyPlayerController.generated.h"

/**
 *
 */
 class ULobbyWidget; 
UCLASS()
class DARKBORNE_API ADBLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_DisplayMessage(const FString& msg);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;
	TObjectPtr<ULobbyWidget> LobbyWidget;
};
