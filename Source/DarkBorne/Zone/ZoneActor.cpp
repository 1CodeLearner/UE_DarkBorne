// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneActor.h"
#include "ZoneNode.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"

AZoneActor::AZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	RootComponent = SMComp;

	totalMoveTime = 0.f;
	maxMoveTime = 12.f;
	bMove = false;
	totalWaitTime = 0.f;
	maxWaitTime = 1.f;
}

void AZoneActor::BeginPlay()
{
	Super::BeginPlay();


}

void AZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMove)
	{
		totalMoveTime += DeltaTime;
		float delta = totalMoveTime / maxMoveTime;
		SetActorLocation(FMath::Lerp<FVector, float>(prevLoc, nextLoc, delta));
		if (totalMoveTime >= maxMoveTime)
		{
			bMove = false;
			totalMoveTime = 0.f;

			AZoneNode* prevNode = Next;
			Next = Next->Next;
			prevNode->SetLifeSpan(2.f);
		}
	}
	else
	{
		totalWaitTime += DeltaTime;
		if (totalWaitTime >= maxWaitTime)
		{
			if (CanMove())
			{
				bMove = true;
				totalWaitTime = 0.f;
				StartMove();
			}
		}
	}
}

bool AZoneActor::CanMove() const
{
	return Next != nullptr;
}

void AZoneActor::StartMove()
{
	nextLoc = Next->GetActorLocation();
	prevLoc = GetActorLocation();
}


