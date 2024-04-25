// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DBLobbyPlayerController.h"

void ADBLobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MaxPlayers = UGameplayStatics::GetIntOption(Options, TEXT("MaxPlayers"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Testing: %d"), MaxPlayers);
}

void ADBLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ActivePlayers.Add(NewPlayer);
	UE_LOG(LogTemp,Warning,TEXT("Connected Players: %d"), ActivePlayers.Num());
	
	if (ActivePlayers.Num() == MaxPlayers) {
		GetWorld()->GetTimerManager().SetTimer(TravelHandle, this, &ADBLobbyGameMode::StartTravel, 2.f, false);
		UE_LOG(LogTemp, Warning, TEXT("Server travel in %f seconds"), 2.f);
		BroadcastMessage(TEXT("Starting Match..."));
	}
	else 
	{
		FString msg = FString::Printf(TEXT("Current players: %d / %d"), ActivePlayers.Num(), MaxPlayers);
		BroadcastMessage(msg);
	}
}

void ADBLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ActivePlayers.Remove(Cast<APlayerController>(Exiting));
	UE_LOG(LogTemp, Warning, TEXT("Connected Players: %d"), ActivePlayers.Num());
	
	if (TravelHandle.IsValid() && ensureAlways(ActivePlayers.Num() < MaxPlayers)) {
		GetWorld()->GetTimerManager().ClearTimer(TravelHandle);
		UE_LOG(LogTemp, Warning, TEXT("Server Travel stopped"));
	}

	FString msg = FString::Printf(TEXT("Current players: %d / %d"), ActivePlayers.Num(), MaxPlayers);
	BroadcastMessage(msg);
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
	FString Option = FString::Printf(TEXT("/Game/DBMaps/NewMap?listen?"));
	GetWorld()->ServerTravel(Option);
}

