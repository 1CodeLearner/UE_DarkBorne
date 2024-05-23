// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DBLobbyPlayerController.generated.h"

/**
 *
 */

UENUM()
enum class ELobbyState
{
	COUNTDOWN UMETA(DisplayName = "CountDown"),
	STARTGAME UMETA(DisplayName = "StartGame"),
	NONE UMETA(DisplayName = "None")
};

class ULobbyWidget;
UCLASS()
class DARKBORNE_API ADBLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADBLobbyPlayerController();

	void Init(float _MaxWaitTime);
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_DisplayMessage(const FString& msg);
	void SetLobbyState(ELobbyState CurrLobbyState);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void ReceivedPlayer() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;
	TObjectPtr<ULobbyWidget> LobbyWidget;

private:
	UFUNCTION()
	void UpdateCountDownTimer(float DeltaTime);
	bool bStartCountDown;
	UPROPERTY(Replicated)
	float MaxWaitTime;
	uint32 CountDownInt;

	float GetServerTime() const;
	UFUNCTION()
	void SyncWithServerTime();
	UFUNCTION(Server, Reliable)
	void Server_RequestTime(float ClientRequestTime);
	UFUNCTION(Client, Reliable)
	void Client_RespondTime(float ServerTime, float ClientRequestTime);
	float CurrSyncTime;
	float MaxSyncTime;
	float ServerClientDeltaTime;

	UFUNCTION()
	void OnRep_LobbyState();
	UPROPERTY(ReplicatedUsing = "OnRep_LobbyState")
	ELobbyState LobbyState;
};
