// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TP_ThirdPersonGameMode.generated.h"

class ADBDropItemManager;
struct FItem;
class ADBItem;
class ADBPlayerController;

DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerUpdateDelegate, ADBPlayerController* /*Player*/, bool /*bExit*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FGameEndDelegate, ADBPlayerController* /*PlayerWon*/);

UCLASS(minimalapi)
class ATP_ThirdPersonGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATP_ThirdPersonGameMode();

	UFUNCTION(BlueprintCallable)
	TArray<FItem> GenerateItems(FName MonsterName);

	UFUNCTION(BlueprintCallable)
	void OnPlayerDead(APlayerController* PlayerController);
	
	TArray<ADBPlayerController*> GetConnectedPlayers() const;

	FPlayerUpdateDelegate OnPlayerUpdate;

	FGameEndDelegate OnGameEnd;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADBDropItemManager> DropItemManager;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleLeavingMap() override;
	virtual void HandleMatchHasEnded() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<ADBDropItemManager> DropItemManagerClass;
	
	//false if player died, true if player is still alive
	TMap<ADBPlayerController*, bool> ActivePlayers;
	//Connected Players
	TArray<ADBPlayerController*> ConnectedPlayers;

	TObjectPtr<ADBPlayerController> WonPlayer; 
	ADBPlayerController* CheckIfPlayerWon();
};



