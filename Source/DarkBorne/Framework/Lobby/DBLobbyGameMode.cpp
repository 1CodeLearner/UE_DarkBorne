// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DBLobbyPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include "../DBDropItemManager.h"


ADBLobbyGameMode::ADBLobbyGameMode()
{
	MaxWaitTime = 15.f;
	RemainingTime = 10000.f;
	bStartCountDown = true;
}

void ADBLobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MaxPlayers = UGameplayStatics::GetIntOption(Options, TEXT("MaxPlayers"), 0);
	startGameWait = 5.f;
	bIsGameStarting = false;

	auto manager = GetWorld()->SpawnActor<ADBDropItemManager>(DropItemManagerClass);
	if (manager)
		DropItemManager = manager;
}

void ADBLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto LobbyPC = Cast<ADBLobbyPlayerController>(NewPlayer);
	if (LobbyPC) 
	{
		LobbyPC->Init(MaxWaitTime);
	}

	ActivePlayers.Add(NewPlayer);
	UE_LOG(LogTemp,Warning,TEXT("Connected Players: %d"), ActivePlayers.Num());
	UE_LOG(LogTemp, Warning, TEXT("PostLogin State Name: %s"), *NewPlayer->GetStateName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("NetId:%s"), *NewPlayer->PlayerState->GetUniqueId().ToString());


	if (ActivePlayers.Num() == MaxPlayers) {
		StartGame(TEXT("Max number of players reached, starting match..."));
	}
	else 
	{
		BroadcastMessage(FString::Printf(TEXT("Current players: %d / %d"), ActivePlayers.Num(), MaxPlayers));
	}
}

void ADBLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	bStartCountDown = true;

	ActivePlayers.Remove(Cast<APlayerController>(Exiting));
	UE_LOG(LogTemp, Warning, TEXT("Connected Players: %d"), ActivePlayers.Num());
	UE_LOG(LogTemp, Warning, TEXT("Logout State Name: %s"), *Exiting->GetStateName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("NetId:%s"), *Exiting->PlayerState->GetUniqueId().ToString());

	if (bIsGameStarting && RemainingTime > 0.f)
	{
		StopTravelToGame();
	}

	if (!bIsGameStarting) {
		BroadcastMessage(FString::Printf(TEXT("Current players: %d / %d"), ActivePlayers.Num(), MaxPlayers));
	}
}

void ADBLobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bStartCountDown) 
	{
		RemainingTime = MaxWaitTime - GetWorld()->GetTimeSeconds();
		if (RemainingTime <= 0.f)
		{
			StartGame(TEXT("Countdown over, starting match..."));
		}
	}	
}

void ADBLobbyGameMode::StartGame(FString Message)
{
	bStartCountDown = false;
	bIsGameStarting = true;
	
	for (int i = 0; i < ActivePlayers.Num(); ++i)
	{
		auto PC = Cast<ADBLobbyPlayerController>(ActivePlayers[i]);
		if (ensureAlways(PC))
		{
			PC->SetLobbyState(ELobbyState::STARTGAME);
		}
	}
	BroadcastMessage(Message);

	GetWorld()->GetTimerManager().SetTimer(TravelHandle, this, &ADBLobbyGameMode::StartTravel, startGameWait, false);
	UE_LOG(LogTemp, Warning, TEXT("Server travel to GameplayMap in %f seconds"), startGameWait);
}

void ADBLobbyGameMode::StopTravelToGame()
{
	bIsGameStarting = false;
	bStartCountDown = true;

	GetWorld()->GetTimerManager().ClearTimer(TravelHandle);

	for (int i = 0; i < ActivePlayers.Num(); ++i)
	{
		auto PC = Cast<ADBLobbyPlayerController>(ActivePlayers[i]);
		if (ensureAlways(PC))
		{
			PC->SetLobbyState(ELobbyState::COUNTDOWN);
		}
	}
}

void ADBLobbyGameMode::BroadcastMessage(FString msg)
{
	for (int i = 0; i < ActivePlayers.Num(); ++i)
	{
		auto PC = Cast<ADBLobbyPlayerController>(ActivePlayers[i]);
		if (ensureAlways(PC))
		{
			PC->Client_DisplayMessage(msg);
		}
	}
}

void ADBLobbyGameMode::StartTravel()
{
	FString Option = FString::Printf(TEXT("/Game/DBMaps/GamePlayMap?listen?"));
	GetWorld()->ServerTravel(Option);
}

ADBDropItemManager* ADBLobbyGameMode::GetItemManager() const
{
	return DropItemManager;
}
