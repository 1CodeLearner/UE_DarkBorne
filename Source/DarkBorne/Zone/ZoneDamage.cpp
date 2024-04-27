// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneDamage.h"
#include "../Framework/DBPlayerController.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Framework/BFL/DarkBorneLibrary.h"

UZoneDamage::UZoneDamage()
{
	PlayerController = nullptr;
	Character = nullptr;

	totalTime = 0.f;
	currTime = 0.f;
	bIsTicking = false;

	damageAmt = 0.f;
}

bool UZoneDamage::Initialize(ADBPlayerController* PC, float TotalTime, float DamageAmount)
{
	if (ensureAlways(PC)) {
		PlayerController = PC;
		auto temp = Cast<ADBCharacter>(PC->GetPawn());
		if (temp) {
			Character = temp;

			this->totalTime = TotalTime;
			damageAmt = DamageAmount;
		}
		else return false;
	}
	else return false;

	return true;
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

bool UZoneDamage::IsTicking() const
{
	return bIsTicking;
}

void UZoneDamage::Tick(float DeltaTime)
{
	if (bIsTicking)
	{
		currTime += DeltaTime;
		if (currTime >= totalTime)
		{
			if (IsValid(Character) && Character->CurrHP > 0.f)
				UDarkBorneLibrary::ApplyDamageAmount(Character, damageAmt);
			currTime = 0.f;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Time: %f"), currTime));
}

TStatId UZoneDamage::GetStatId() const
{
	return TStatId();
}

UWorld* UZoneDamage::GetWorld() const
{
	Super::GetWorld();

	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}
