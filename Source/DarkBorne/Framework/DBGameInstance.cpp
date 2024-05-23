// Fill out your copyright notice in the Description page of Project Settings.


#include "DBGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Misc/Guid.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
void UDBGameInstance::Init()
{
	Super::Init();


	// 온라인 서브 시스템 가져오자
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		// 세션 인터페이스 가져오자
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDBGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UDBGameInstance::OnFindSessionComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UDBGameInstance::OnJoinSessionComplete);
	}
	FGuid guid;
	roomName = guid.NewGuid().ToString();
	maxPlayer = 3;

	DestroyMySession();

}

void UDBGameInstance::CreateMySession()
{
	FOnlineSessionSettings sessionSettings;

	// true 세션이 검색 된다.
	sessionSettings.bShouldAdvertise = true;

	// steam 사용하면 해당 옵션이 true 세션을 만들 수 있다.
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 내가 게임중인 아닌지를 보여줄건지
	sessionSettings.bUsesPresence = true;
	// 게임 플레이 중에 참여할 수 있게
	sessionSettings.bAllowJoinInProgress = true;
	sessionSettings.bAllowJoinViaPresence = true;

	// 인원 수 
	sessionSettings.NumPublicConnections = maxPlayer;

	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	// 세션 생성 요청
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	int32 rand = FMath::RandRange(1, 100000);
	mySessionName += FString::Printf(TEXT("%d"), rand);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UDBGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnCreateComplete.ExecuteIfBound(true);

		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());
		// Battle Map 으로 이동하자
		FString Option = FString::Printf(TEXT("/Game/DBMaps/LobbyMap?listen?MaxPlayers=%d"), maxPlayer);
		GetWorld()->ServerTravel(Option);
	}
	else
	{
		OnCreateComplete.ExecuteIfBound(false);
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}

void UDBGameInstance::DestroyMySession()
{
	sessionInterface->DestroySession(FName(mySessionName));
}

void UDBGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

void UDBGameInstance::FindOtherSession()
{
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionSearch->MaxSearchResults = 10;

	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("NetId:%s"), *PC->PlayerState->GetUniqueId().ToString());

	// 세션 검색 요청
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	UE_LOG(LogTemp, Warning, TEXT("Searching session"));
}

void UDBGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Success - count : %d"), results.Num());

		int roomIndex = -1;

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult si = results[i];
			si.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);

			// 세션 정보 ---> String 으로 
			int32 max = si.Session.SessionSettings.NumPublicConnections;

			int32 currPlayer = max - si.Session.NumOpenPublicConnections;
			//room is full
			if (currPlayer >= max)
				continue;

			//
			roomIndex = i;
			break;
		}

		if (roomIndex != -1)
		{
			OnFindComplete.ExecuteIfBound(true, true);
			JoinOtherSession(roomIndex);
		}
		else {
			OnFindComplete.ExecuteIfBound(true, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Fail"));
		OnFindComplete.ExecuteIfBound(false, false);
	}
}

void UDBGameInstance::JoinOtherSession(int32 idx)
{
	//TArray<FOnlineSessionSearchResult> 
	auto results = sessionSearch->SearchResults;
	if (sessionInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("sessionInterface is null"));
	}
	if (results.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("results Zero"));
	}
	UE_LOG(LogTemp, Warning, TEXT("results count : %d, idx : %d"), results.Num(), idx);
	sessionInterface->JoinSession(0, FName(mySessionName), results[idx]);
}

void UDBGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		OnJoinSessionEvent.ExecuteIfBound(true);

		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success : %s"), *SessionName.ToString());
		FString url;
		// 참여해야 하는 Listen 서버 URL을 받아 오자
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);

		if (!url.IsEmpty())
		{
			// 해당 URL 로 접속하자
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		OnJoinSessionEvent.ExecuteIfBound(false);
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Fail : %d"), result);
	}
}

void UDBGameInstance::Shutdown()
{
	Super::Shutdown();

	DestroyMySession();
}