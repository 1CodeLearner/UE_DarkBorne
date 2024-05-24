// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DBLobbyGameMode.generated.h"

/**
 *
 */
class APlayerController;
class ADBDropItemManager;
UCLASS()
class DARKBORNE_API ADBLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADBLobbyGameMode();
	ADBDropItemManager* GetItemManager() const;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<ADBDropItemManager> DropItemManagerClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADBDropItemManager> DropItemManager;
private:
	void StartGame(FString Message);
	void StopTravelToGame();
	 
	TArray<APlayerController*> ActivePlayers;
	int32 MaxPlayers;
	void BroadcastMessage(FString msg);

	FTimerHandle TravelHandle;
	float startGameWait;
	bool bIsGameStarting;

	UFUNCTION()
	void StartTravel();

	float MaxWaitTime;
	float RemainingTime;
	bool bStartCountDown;
};
