// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TP_ThirdPersonGameMode.generated.h"

class ADBDropItemManager;
struct FItem;
class ADBItem;
class ADBPlayerController;

UCLASS(minimalapi)
class ATP_ThirdPersonGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATP_ThirdPersonGameMode();

	UFUNCTION(BlueprintCallable)
	TArray<FItem> GenerateItems(FName MonsterName);

	UFUNCTION(BlueprintCallable)
	ADBItem* SpawnItem(AActor* Instigated, FItem Item);

	UFUNCTION(BlueprintCallable)
	void OnPlayerDead(APlayerController* PlayerController);


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
	TObjectPtr<ADBDropItemManager> DropItemManager;
	
	TMap<ADBPlayerController*, bool> ActivePlayers;
	TObjectPtr<ADBPlayerController> WonPlayer; 
	ADBPlayerController* CheckIfPlayerWon();
};



