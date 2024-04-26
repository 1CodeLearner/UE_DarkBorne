// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneDamage.h"
#include "../Framework/DBPlayerController.h"
#include "../DBCharacters/DBCharacter.h"

UZoneDamage::UZoneDamage()
{
	PlayerController = nullptr;
	Character = nullptr;

	totalTime = 0.f;
	currTime = 0.f;
	bIsTicking = false;
}

void UZoneDamage::Initialize(ADBPlayerController* PC)
{
	if (ensureAlways(PC)) {
		PlayerController = PC;
		auto temp = Cast<ADBCharacter>(PC->GetPawn());
		if (temp) {
			Character = temp;
		}
	}
}

void UZoneDamage::UpdateTotalTime(float newTotalTime)
{
	totalTime = newTotalTime;
}

void UZoneDamage::StartTick()
{
	bIsTicking = true;
}

void UZoneDamage::StopTick()
{
	bIsTicking = false;
	currTime = 0.f;
}

void UZoneDamage::Tick(float DeltaTime)
{
	
}

TStatId UZoneDamage::GetStatId() const
{
	return TStatId();
}

UWorld* UZoneDamage::GetWorld() const
{
	Super::GetWorld();

	AActor* Actor =Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}
