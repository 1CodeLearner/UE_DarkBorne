// Fill out your copyright notice in the Description page of Project Settings.


#include "DBEffect.h"
#include "../Inventory/ItemObject.h"
#include "../DBCharacters/DBCharacter.h"
#include "DBEffectComponent.h"
#include "Net/UnrealNetwork.h"

void UDBEffect::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Parent's Tick is in effect %s"),
		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER")
	));

	//Only ticks on server
	Time.currTime += DeltaTime;
	if (Time.currTime >= Time.TotalTime)
	{
		//Update UI
		OnRep_Time();
		Time.currTime = 0.f;
		GetEffectComponent()->RemoveEffect(this);
	}
	else
	{
		OnRep_Time();
	}
}

void UDBEffect::OnRep_Time()
{
	OnEveryTick.ExecuteIfBound(Time.TotalTime, Time.currTime);
}

void UDBEffect::Initialize(ADBCharacter* Instigator, UItemObject* Item, UDBEffectComponent* EffectComp)
{
	if (Instigator && Item && EffectComp)
	{
		AffectedCharacter = Instigator;
		Id = Item->GetId();
		EffectComponent = EffectComp;
		IconDisplay = Item->GetIcon();
	}
}

void UDBEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(UDBEffect, Time, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(UDBEffect, IconDisplay, COND_OwnerOnly);
	DOREPLIFETIME(UDBEffect, Time);
	DOREPLIFETIME(UDBEffect, IconDisplay);
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
	//OnStop.ExecuteIfBound();
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

UMaterialInterface* UDBEffect::GetIcon() const
{
	return IconDisplay;
}
