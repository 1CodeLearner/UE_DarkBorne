// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffect.h"
#include "../Inventory/ItemObject.h"
#include "../DBCharacters/DBCharacter.h"
#include "DBEffectComponent.h"

void UDBEffect::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Parent's Tick is in effect")));
	currTime += DeltaTime;
	if (currTime >= TotalTime)
	{
		//Update UI
		OnEveryTick.ExecuteIfBound(TotalTime, currTime);
		currTime = 0.f;
		GetEffectComponent()->RemoveEffect(this);
	}
	else
	{
		OnEveryTick.ExecuteIfBound(TotalTime, currTime);
	}
}

void UDBEffect::Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp)
{
	if (Instigator && Item && EffectComp)
	{
		AffectedCharacter = Instigator;
		Id = Item->GetId();
		EffectComponent = EffectComp;
	}
}

UDBEffectComponent* UDBEffect::GetEffectComponent() const
{
	return EffectComponent;
}

TStatId UDBEffect::GetStatId() const
{
	return TStatId();
}

void UDBEffect::StartTick()
{
	bIsTicking = true;
}

void UDBEffect::StopTick()
{
	bIsTicking = false;
	//Remove Effect Icon from UI
	OnStop.ExecuteIfBound();
}

bool UDBEffect::IsTicking()
{
	return bIsTicking;
}

bool UDBEffect::IsSame(UDBEffect* OtherEffect) const
{
	return OtherEffect->GetId() == OtherEffect->GetId();
}

bool UDBEffect::IsSame(FName OtherId) const
{
	return Id == OtherId;
}

FName UDBEffect::GetId() const
{
	return Id;
}
