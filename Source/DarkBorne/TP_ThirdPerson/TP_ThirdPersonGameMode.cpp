// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../Framework/DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"
#include "../ItemTypes/EnchantmentTypes.h"
#include "../Framework/DBPlayerController.h"

ATP_ThirdPersonGameMode::ATP_ThirdPersonGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

TArray<FItem> ATP_ThirdPersonGameMode::GenerateItems(FName MonsterName)
{
	if (ensureAlways(DropItemManager)) {
		return DropItemManager->GenerateItems(MonsterName);
	}
	return TArray<FItem>();
}

ADBItem* ATP_ThirdPersonGameMode::SpawnItem(AActor* Instigated, FItem Item)
{
	if (ensureAlways(DropItemManager)) {
		return DropItemManager->SpawnItem(Instigated, Item);
	}
	return nullptr;
}

void ATP_ThirdPersonGameMode::OnPlayerDead(APlayerController* PlayerController)
{
	auto DBPC = Cast<ADBPlayerController>(PlayerController);
	if (DBPC && ensureAlways(ActivePlayers.Find(DBPC)))
	{
		//declare dead
		ActivePlayers[DBPC] = false;
		WonPlayer = CheckIfPlayerWon();
		if(WonPlayer)
			EndMatch(); 
	}
}

TArray<ADBPlayerController*> ATP_ThirdPersonGameMode::GetConnectedPlayers() const
{
	return ConnectedPlayers;
}

void ATP_ThirdPersonGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	auto manager = GetWorld()->SpawnActor<ADBDropItemManager>(DropItemManagerClass);
	if (manager)
		DropItemManager = manager;
}

void ATP_ThirdPersonGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);	

	for (ADBPlayerController* What : ConnectedPlayers)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("PC: %s"), *GetNameSafe(What)));
	}
}

void ATP_ThirdPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATP_ThirdPersonGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto PC = Cast<ADBPlayerController>(NewPlayer);

	ActivePlayers.Add(PC, true);
	ConnectedPlayers.Add(PC);
	OnPlayerUpdate.Broadcast(PC, false);
}

void ATP_ThirdPersonGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	auto PC = Cast<ADBPlayerController>(Exiting);
	if (PC) 
	{
		ActivePlayers.Remove(PC);
		ConnectedPlayers.Add(PC);
		OnPlayerUpdate.Broadcast(PC, true);

		WonPlayer = CheckIfPlayerWon();
		if (WonPlayer)
			EndMatch();
	}
}

void ATP_ThirdPersonGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

}

void ATP_ThirdPersonGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ATP_ThirdPersonGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

void ATP_ThirdPersonGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	UE_LOG(LogTemp, Warning, TEXT("Player won : %s"), *WonPlayer->GetName());
	
	for (const auto& what : ActivePlayers)
	{
		//Tell each controller to display their match state.
		what.Key->Client_DisplayGameResult(what.Value);
	}
}

ADBPlayerController* ATP_ThirdPersonGameMode::CheckIfPlayerWon()
{
	int count = 0;
	TPair<ADBPlayerController*, bool> save{nullptr, false};
	for (const auto& what : ActivePlayers)
	{
		if (what.Value)
		{
			count++;
			save = what;
		}
	}
	if (count == 1) return save.Key;
	
	return nullptr;
}

