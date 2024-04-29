// Fill out your copyright notice in the Description page of Project Settings.


#include "DBPlayerController.h"
#include "../UI/GameEndWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "DBDropItemManager.h"
#include "../ItemTypes/EnchantmentTypes.h"
#include "../ItemTypes/ItemType.h"
#include "../Inventory/ItemObject.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"

void ADBPlayerController::Client_DisplayGameResult_Implementation(bool bHasWon)
{
	if (ensureAlways(GameEndWidgetClass) && !GameEndWidget)
	{
		GameEndWidget = CreateWidget<UGameEndWidget>(this, GameEndWidgetClass);
	}

	if (GameEndWidget)
	{
		if (bHasWon) {
			GameEndWidget->SetGameStateText(FText::FromString(FString::Printf(TEXT("WON"))));
		}
		else
			GameEndWidget->SetGameStateText(FText::FromString(FString::Printf(TEXT("LOST"))));

		GameEndWidget->AddToViewport();
	}
}

void ADBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController()) 
	{
		UE_LOG(LogTemp,Warning, TEXT("YEs %s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NOOE"));
	}
}

void ADBPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (HasAuthority()) {
		UItemObject* ItemObject = nullptr;

		auto GameplayGM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
		if (GameplayGM) {
			FItem Item = GameplayGM->DropItemManager->GenerateItemByName(FName("Dagger"), EItemType::WEAPON);
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
				EquipComp->Server_AddItem(ItemObject);
			}
		}
	}
}
