// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBEffect.generated.h"

class ADBCharacter;
class UItemObject;
class UDBEffectComponent;

DECLARE_DELEGATE(FEverytickDelegate, TotalTime, RemainingTime)
DECLARE_DELEGATE(FStopDelegate);

UCLASS()
class DARKBORNE_API UDBEffect : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	void Initialize(ADBCharacter* Instigator, UItemObject* Item);
	void StartTick();
	void StopTick();
	bool IsTicking();

	bool IsSame(UDBEffect* OtherEffect) const;
	FName GetId() const;
	 
	FEverytickDelegate OnEveryTick;
	FStopDelegate OnStop;

protected:
	UDBEffectComponent* GetEffectComponent() const;

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<ADBCharacter> AffectedCharacter;

	float TotalTime;
	float currTime;
private:
	bool bIsTicking;

	UPROPERTY()
	TObjectPtr<UDBEffectComponent> EffectComponent;
	FName Id;
};
