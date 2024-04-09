// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../Framework/DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"

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

TArray<UPDA_ItemSlot*> ATP_ThirdPersonGameMode::GenerateItems(FName MonsterName)
{
	TArray<UPDA_ItemSlot*> Generated;
	if (ensureAlways(DropItemManager)) {
		Generated = DropItemManager->GenerateItems(MonsterName);
	}

	return Generated;
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

	UE_LOG(LogTemp,Warning, TEXT("What's up? %s"), *GetNameSafe(DropItemManager));
}

