// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffect.h"
#include "../Inventory/ItemObject.h"
#include "../DBCharacters/DBCharacter.h"

void UDBEffect::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Tick Effect Parent")));
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

bool UDBEffect::IsSame(FName OtherId) const
{
	return Id == OtherId;
}

FName UDBEffect::GetId() const
{
	return Id;
}
