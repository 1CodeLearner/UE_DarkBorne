// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../Framework/DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"
#include "../ItemTypes/EnchantmentTypes.h"
#include "../Framework/DBPlayerController.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/ItemObject.h"
#include "../Inventory/BaseInventoryComponent.h"

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

void ATP_ThirdPersonGameMode::AddItemTo(AActor* Actor, FName LootName)
{	
	auto InventoryComp = Actor->GetComponentByClass<UBaseInventoryComponent>();
	if (InventoryComp)
	{
		TArray<FItem> Items = GenerateItems(LootName);
		for (int i = 0; i < Items.Num(); ++i)
		{
			auto ItemObject = NewObject<UItemObject>(this);
			ItemObject->Initialize(Items[i]);

			UE_LOG(LogTemp, Warning, TEXT("Yes it works"));
			InventoryComp->TryAddItem(ItemObject, Actor);
		}
	}
}

TArray<FItem> ATP_ThirdPersonGameMode::GenerateItems(FName MonsterName)
{
	if (ensureAlways(DropItemManager)) {
		return DropItemManager->GenerateItems(MonsterName);
	}
	return TArray<FItem>();
}

void ATP_ThirdPersonGameMode::OnPlayerDead(APlayerController* PlayerController)
{
	auto DBPC = Cast<ADBPlayerController>(PlayerController);
	UE_LOG(LogTemp,Warning,TEXT("Inside OnPlayerDead"));
	if (DBPC && ensureAlways(ActivePlayers.Find(DBPC)))
	{
		//declare dead
		ActivePlayers[DBPC] = false;
		WonPlayer = CheckIfPlayerWon();
		if (WonPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found WonPlayer in OnPlayerDead: %s"), *GetNameSafe(WonPlayer));
			OnGameEnd.Broadcast(WonPlayer);
			EndMatch();
		}
	}
}

TArray<ADBPlayerController*> ATP_ThirdPersonGameMode::GetConnectedPlayers() const
{
	return ConnectedPlayers;
}

TArray<class ADBCharacter*> ATP_ThirdPersonGameMode::GetPlayerCharacters()
{
	// 플레이어 캐릭터를 저장할 배열
	TArray<class ADBCharacter*> PlayerCharacters;

	// ActivePlayers 맵을 순회하며 각 플레이어에게 할당된 캐릭터 가져오기
	for (auto& PlayerPair : ActivePlayers)
	{
		ADBPlayerController* PlayerController = PlayerPair.Key;
		bool bIsPlayerAlive = PlayerPair.Value;

		if (PlayerController && bIsPlayerAlive)
		{
			ADBCharacter* PlayerCharacter = Cast<ADBCharacter>(PlayerController->GetPawn());
			if (PlayerCharacter)
			{
				PlayerCharacters.Add(PlayerCharacter);
			}
		}
	}

	return PlayerCharacters;
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
}

void ATP_ThirdPersonGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ATP_ThirdPersonGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp,Warning,TEXT("Inside PostLogin: %s"), *GetNameSafe(NewPlayer));

	auto PC = Cast<ADBPlayerController>(NewPlayer);

	ActivePlayers.Add(PC, true);
	ConnectedPlayers.Add(PC);
	OnPlayerUpdate.Broadcast(PC, false);
}

void ATP_ThirdPersonGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	UE_LOG(LogTemp, Warning, TEXT("Inside Logout: %s"), *GetNameSafe(Exiting));

	auto PC = Cast<ADBPlayerController>(Exiting);
	if (PC)
	{
		ActivePlayers.Remove(PC);
		ConnectedPlayers.Add(PC);
		OnPlayerUpdate.Broadcast(PC, true);

		WonPlayer = CheckIfPlayerWon();
		if (WonPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found WonPlayer in Logout: %s"), *GetNameSafe(WonPlayer));
			EndMatch();
		}
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

	for (const auto& what : ActivePlayers)
	{
		if (what.Value) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Inside HandlematchHasEnded, Player won : %s"), *WonPlayer->GetName());
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Inside HandlematchHasEnded, Player lost : %s"), *WonPlayer->GetName());
		}
		//Tell each controller to display their match state.
		what.Key->Client_DisplayGameResult(what.Value);
	}
}

ADBPlayerController* ATP_ThirdPersonGameMode::CheckIfPlayerWon()
{
	int count = 0;
	ADBPlayerController* PlayerWon = nullptr;
	for (const auto& what : ActivePlayers)
	{
		if (what.Value)
		{
			count++;
			PlayerWon = what.Key;
		}
	}
	UE_LOG(LogTemp,Warning,TEXT("Inside CheckIfPlayerWon"));
	if (PlayerWon && count == 1) return PlayerWon;

	return nullptr;
}
