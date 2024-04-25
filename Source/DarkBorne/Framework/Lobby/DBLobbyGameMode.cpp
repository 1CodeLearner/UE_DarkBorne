// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyGameMode.h"
#include "Kismet/GameplayStatics.h"

void ADBLobbyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	int val = UGameplayStatics::GetIntOption(Options, TEXT("PlayerCount"), 0);
	UE_LOG(LogTemp, Warning, TEXT("Testing: %d"), val);
}

void ADBLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ActivePlayers.Add(NewPlayer);
	UE_LOG(LogTemp,Warning,TEXT("Connected Players: %d"), ActivePlayers.Num());
	
}

void ADBLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	ActivePlayers.Remove(Cast<APlayerController>(Exiting));
	UE_LOG(LogTemp, Warning, TEXT("Connected Players: %d"), ActivePlayers.Num());
}

