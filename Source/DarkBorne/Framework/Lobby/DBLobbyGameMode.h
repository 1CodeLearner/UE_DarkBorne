// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBLobbyGameMode.generated.h"

/**
 *
 */
 class APlayerController;
UCLASS()
class DARKBORNE_API ADBLobbyGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
private:
	TArray<APlayerController*> ActivePlayers;
	int32 MaxPlayers;
	void BroadcastMessage(FString msg);

	FTimerHandle TravelHandle;
	float startGameWait;

	UFUNCTION()
	void StartTravel();
};
