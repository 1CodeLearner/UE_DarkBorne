// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DBGameInstance.generated.h"

/**
 *
 */

DECLARE_DELEGATE_TwoParams(FFindCompleteDelegate, bool /*bWasSuccessful*/, bool /*bCanJoinSession*/);
DECLARE_DELEGATE_OneParam(FJoinSessionEventDelegate, bool /*bWasSuccessful*/);
DECLARE_DELEGATE_OneParam(FCreateCompleteDelegate, bool /*bWasSuccessful*/);

UCLASS()
class DARKBORNE_API UDBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// 이 변수통해 (세션 만들고, 세션 검색, 세션 참여)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// 세션 검색이 완료되면 호출해야 하는 Delegate
	FFindCompleteDelegate OnFindComplete;
	FJoinSessionEventDelegate OnJoinSessionEvent;
	FCreateCompleteDelegate OnCreateComplete;

	// 세션 만드는 함수
	UFUNCTION(BlueprintCallable)
	void CreateMySession(int32 PlayerCount, float CountdownTime);
			
	// 세션을 검색 함수
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();

	void DestroyMySession();

protected:
	virtual void Init() override;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여 함수
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	virtual void Shutdown() override;
protected:

	// 세션 검색에 쓰이는 클래스
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// 세션 이름
	FString mySessionName = TEXT("SessionFind");

	FString roomName;
	int32 maxPlayer;
	float CountdownTime;
};
