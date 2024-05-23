// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DBLobbyPlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>

void ADBLobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	MaxPlayers = UGameplayStatics::GetIntOption(Options, TEXT("MaxPlayers"), 0);
	startGameWait = 5.f;
	bIsGameStarting = false;
}

void ADBLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ActivePlayers.Add(NewPlayer);
	UE_LOG(LogTemp,Warning,TEXT("Connected Players: %d"), ActivePlayers.Num());
	UE_LOG(LogTemp, Warning, TEXT("PostLogin State Name: %s"), *NewPlayer->GetStateName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("NetId:%s"), *NewPlayer->PlayerState->GetUniqueId().ToString());


	if (ActivePlayers.Num() == MaxPlayers) {
		GetWorld()->GetTimerManager().SetTimer(TravelHandle, this, &ADBLobbyGameMode::StartTravel, startGameWait, false);
		UE_LOG(LogTemp, Warning, TEXT("Server travel to GameplayMap in %f seconds"), startGameWait);
		BroadcastMessage(TEXT("Starting Match..."));
		bIsGameStarting = true;
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
	UE_LOG(LogTemp, Warning, TEXT("Logout State Name: %s"), *Exiting->GetStateName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("NetId:%s"), *Exiting->PlayerState->GetUniqueId().ToString());

	/*if (TravelHandle.IsValid() && ensureAlways(ActivePlayers.Num() < MaxPlayers)) {
		GetWorld()->GetTimerManager().ClearTimer(TravelHandle);
		UE_LOG(LogTemp, Warning, TEXT("Server Travel stopped"));
	}*/

	if (!bIsGameStarting) {
		FString msg = FString::Printf(TEXT("Current players: %d / %d"), ActivePlayers.Num(), MaxPlayers);
		BroadcastMessage(msg);
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

