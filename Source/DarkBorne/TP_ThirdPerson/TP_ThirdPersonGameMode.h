// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TP_ThirdPersonGameMode.generated.h"

class ADBDropItemManager;
struct FItem;

UCLASS(minimalapi)
class ATP_ThirdPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATP_ThirdPersonGameMode();

	UFUNCTION(BlueprintCallable)
	TArray<FItem> GenerateItems(FName MonsterName);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<ADBDropItemManager> DropItemManagerClass;

private:
	TObjectPtr<ADBDropItemManager> DropItemManager;
};



