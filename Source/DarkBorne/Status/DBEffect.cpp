// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffect.h"
#include "../Inventory/ItemObject.h"
#include "../DBCharacters/DBCharacter.h"

void UDBEffect::Tick(float DeltaTime)
{
	if (bIsTicking)
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf("Tick Effect Parent"));
}

void UDBEffect::Initialize(ADBCharacter* Instigator, UItemObject* Item)
{
	if (Instigator && Item)
	{
		AffectedCharacter = Instigator;
		Id = Item->GetId();
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

FName UDBEffect::GetId() const
{
	return Id;
}
