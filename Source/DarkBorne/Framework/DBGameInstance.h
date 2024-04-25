// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "DBGameInstance.generated.h"

/**
 *
 */

DECLARE_DELEGATE_OneParam(FSearchComplete, int32 /*num of sessions*/);

UCLASS()
class DARKBORNE_API UDBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	// ���� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	void CreateMySession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// ������ �˻� �Լ�
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// ���� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);


public:
	// �� �������� (���� �����, ���� �˻�, ���� ����)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// ���� �˻��� ���̴� Ŭ����
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// ���� �̸�
	FString mySessionName = TEXT("SessionFind");

	// ���� �˻��� �Ϸ�Ǹ� ȣ���ؾ� �ϴ� Delegate
	FSearchComplete onSearchComplete;

	FString roomName;
	int32 maxPlayer;
};
