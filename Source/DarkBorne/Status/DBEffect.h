// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DBEffect.generated.h"

class ADBCharacter;
class UItemObject;
class UDBEffectComponent;
class UMaterialInterface;

USTRUCT()
struct FTime
{
	GENERATED_BODY()
	
	UPROPERTY()
	float TotalTime = 0.f;
	UPROPERTY()
	float currTime = 0.f;
};


DECLARE_DELEGATE_TwoParams(FEverytickDelegate, float /*TotalTime*/, float /*RemainingTime*/);
//DECLARE_DELEGATE(FStopDelegate);

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
	
	UMaterialInterface* GetIcon() const;

	FEverytickDelegate OnEveryTick;
	//FStopDelegate OnStop;

protected:
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UWorld* GetWorld() const override;

protected:
	UDBEffectComponent* GetEffectComponent() const;

	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsAllowedToTick() const override { return bIsTicking; }

	TObjectPtr<ADBCharacter> AffectedCharacter;

	UPROPERTY(ReplicatedUsing = "OnRep_Time")
	FTime Time;
	UFUNCTION()
	void OnRep_Time();
	UPROPERTY(Replicated)
	TObjectPtr<UMaterialInterface> IconDisplay;

private:
	bool bIsTicking;

	UPROPERTY()
	TObjectPtr<UDBEffectComponent> EffectComponent;
	FName Id;
};
