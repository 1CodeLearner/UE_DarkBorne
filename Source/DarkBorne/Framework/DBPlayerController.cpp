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
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpectatorPawn.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

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

void ADBPlayerController::ChangeToSpectator()
{
	ServerRPC_ChangeToSpectator();
}

void ADBPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	APawn* player = GetPawn();
	if (player)
	{
		// 플레이 게임 모드의 게임모드를 가져오기
		ATP_ThirdPersonGameMode* gm = Cast<ATP_ThirdPersonGameMode>(GetWorld()->GetAuthGameMode());

		// 관전자 Pawn 생성
		ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, player->GetActorTransform());

		// 현재 플레이어 Possess 해제
		UnPossess();

		// 관전자 Pawn 을 Possess
		Possess(spectator);
		//UKismetSystemLibrary::K2_SetTimer(this, TEXT("ChangeToSpectator"), 5, false);
	}
}
