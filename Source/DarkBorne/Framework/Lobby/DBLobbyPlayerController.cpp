// Fill out your copyright notice in the Description page of Project Settings.


#include "DBLobbyPlayerController.h"
#include "../../UI/LobbyWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "DBLobbyGameMode.h"
#include "../DBDropItemManager.h"
#include "../../ItemTypes/EnchantmentTypes.h"
#include "../../Inventory/ItemObject.h"
#include "../../DBCharacters/DBCharacter.h"
#include "../../Inventory/DBEquipmentComponent.h"
#include "../DBDropItemManager.h"
#include "Net/UnrealNetwork.h"


ADBLobbyPlayerController::ADBLobbyPlayerController()
{
	MaxWaitTime = 0.f;
	CountDownInt = 0;
	bStartCountDown = false;

	CurrSyncTime = 0.f;
	MaxSyncTime = 3.f;

	ServerClientDeltaTime = 0.f;

	LobbyState = ELobbyState::COUNTDOWN;
}

void ADBLobbyPlayerController::Init(float _MaxWaitTime)
{
	MaxWaitTime = _MaxWaitTime;
}

void ADBLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
}

void ADBLobbyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	if (HasAuthority()) {
		UItemObject* ItemObject = nullptr;

		auto LobbyGM = GetWorld()->GetAuthGameMode<ADBLobbyGameMode>();
		if (LobbyGM && LobbyGM->GetItemManager()) {
			ItemObject = LobbyGM->GetItemManager()->GenerateItemByName(FName("Dagger"), EItemType::WEAPON);
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

void ADBLobbyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Ticking...")));
	if (IsLocalPlayerController())
	{
		if (bStartCountDown) {
			UpdateCountDownTimer(DeltaSeconds);
		}

		if (!HasAuthority())
		{
			CurrSyncTime += DeltaSeconds;
			if (CurrSyncTime > MaxSyncTime)
			{
				CurrSyncTime = 0.f;
				SyncWithServerTime();
			}
		}
	}
}

void ADBLobbyPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	SyncWithServerTime();
}

void ADBLobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ADBLobbyPlayerController, MaxWaitTime, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ADBLobbyPlayerController, LobbyState, COND_OwnerOnly);
}

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

void ADBLobbyPlayerController::UpdateCountDownTimer(float DeltaTime)
{
	uint32 CurrWaitTimeInt = FMath::CeilToInt(MaxWaitTime - GetServerTime());
	if (LobbyWidget && CountDownInt != CurrWaitTimeInt)
	{
		LobbyWidget->DisplayTimer(float(CurrWaitTimeInt));
		CountDownInt = CurrWaitTimeInt;
	}

	if (CountDownInt == 0)
	{
		LobbyWidget->DisplayTimer(float(0));
	}
}

float ADBLobbyPlayerController::GetServerTime() const
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return ServerClientDeltaTime + GetWorld()->GetTimeSeconds();
}

void ADBLobbyPlayerController::SyncWithServerTime()
{
	Server_RequestTime(GetWorld()->GetTimeSeconds());
}

void ADBLobbyPlayerController::SetLobbyState(ELobbyState CurrLobbyState)
{
	LobbyState = CurrLobbyState;
	OnRep_LobbyState();
}

void ADBLobbyPlayerController::OnRep_LobbyState()
{
	switch (LobbyState)
	{
	case ELobbyState::COUNTDOWN:
	{
		if (LobbyWidget)
		{
			LobbyWidget->EnableMessage(true);
			LobbyWidget->EnableTimer(true);
		}
		bStartCountDown = true;
		EnableInput(this);
		break;
	}
	case ELobbyState::STARTGAME:
	{
		if (LobbyWidget)
		{
			LobbyWidget->EnableMessage(true);
			LobbyWidget->EnableTimer(false);
		}
		bStartCountDown = false;
		DisableInput(this);
		break;
	}
	}
}

void ADBLobbyPlayerController::Server_RequestTime_Implementation(float ClientRequestTime)
{
	Client_RespondTime(GetWorld()->GetTimeSeconds(), ClientRequestTime);
}

void ADBLobbyPlayerController::Client_RespondTime_Implementation(float ServerTime, float ClientRequestTime)
{
	float EstimatedHalfTravelTime = (GetWorld()->GetTimeSeconds() - ClientRequestTime) / 2.f;

	ServerClientDeltaTime = (ServerTime - EstimatedHalfTravelTime) - GetWorld()->GetTimeSeconds();

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("DeltaTime : %f"), ServerClientDeltaTime));

	if (!bStartCountDown)
	{
		bStartCountDown = true;
	}
}
