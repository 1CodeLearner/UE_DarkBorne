// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyPlayerController.h"
#include "../../UI/LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "DBLobbyGameMode.h"
#include "../DBDropItemManager.h"
#include "../../ItemTypes/EnchantmentTypes.h"
#include "../../ItemTypes/ItemType.h"
#include "../../Inventory/ItemObject.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../../Inventory/DBEquipmentComponent.h"

void ADBLobbyPlayerController::Client_DisplayMessage_Implementation(const FString& msg)
{
	if (ensureAlways(LobbyWidgetClass) && !LobbyWidget)
	{
		LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		LobbyWidget->AddToViewport();
	}

	if (LobbyWidget)
	{
		LobbyWidget->DisplayMessage(msg);
	}
}

void ADBLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		/*UE_LOG(LogTemp, Warning, TEXT("YEs %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));*/
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NOOE"));
	}
}

void ADBLobbyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (HasAuthority()) {
		UItemObject* ItemObject = nullptr;

		auto LobbyGM = GetWorld()->GetAuthGameMode<ADBLobbyGameMode>();
		if (LobbyGM) {
			FItem Item = LobbyGM->DropItemManager->GenerateItemByName(FName("Dagger"), EItemType::WEAPON);
			if (Item.IsValid()) {
				ItemObject = NewObject<UItemObject>(this);
				ItemObject->Initialize(Item);
			}
		}

		if (!ItemObject) return;
		
		auto Charac = Cast<ADBCharacter>(aPawn);
		if (Charac) {
			auto EquipComp = Charac->GetComponentByClass<UDBEquipmentComponent>();
			if (EquipComp) {
				EquipComp->Server_AddItem(ItemObject, Charac);
			}
		}
	}
}
