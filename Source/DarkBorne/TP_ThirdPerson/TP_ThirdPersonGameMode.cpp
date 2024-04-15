// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "../Framework/DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"
#include "../ItemTypes/EnchantmentTypes.h"

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

