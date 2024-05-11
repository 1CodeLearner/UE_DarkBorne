// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBEffect.generated.h"

class ADBCharacter;
class UItemObject;
class UDBEffectComponent;

DECLARE_DELEGATE_TwoParams(FEverytickDelegate, float /*TotalTime*/, float /*RemainingTime*/);
DECLARE_DELEGATE(FStopDelegate);

UCLASS(Blueprintable)
class DARKBORNE_API UDBEffect : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	virtual void Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp);
	virtual void StartTick();
	virtual void StopTick();
	bool IsTicking();

	bool IsSame(UDBEffect* OtherEffect) const;
	bool IsSame(FName OtherId) const;
	FName GetId() const;
	 
	FEverytickDelegate OnEveryTick;
	FStopDelegate OnStop;

protected:
	UDBEffectComponent* GetEffectComponent() const;

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsAllowedToTick() const override { return bIsTicking; }

	TObjectPtr<ADBCharacter> AffectedCharacter;
	
	float TotalTime;
	float currTime;
private:
	bool bIsTicking;

	UPROPERTY()
	TObjectPtr<UDBEffectComponent> EffectComponent;
	FName Id;
};
