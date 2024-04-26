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

	currMoveTime = 0.f;
	maxMoveTime = 5.f;
	bMove = false;
	currWaitTime = 0.f;
	maxWaitTime = 1.f;
	index = -1;
}

void AZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<AZoneNode>it(GetWorld()); it; ++it)
	{
		Nodes.Add(*it);
	}
	//UE_LOG(LogTemp, Warning, TEXT("What"));
	currScale = GetActorScale3D();
	UE_LOG(LogTemp, Warning, TEXT("scale: %s"), *currScale.ToString());

	diffScale = currScale / Nodes.Num();
	UE_LOG(LogTemp, Warning, TEXT("scale: %s"), *diffScale.ToString());
}

void AZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMove)
	{
		currMoveTime += DeltaTime;
		float delta = currMoveTime / maxMoveTime;
		float X = FMath::Lerp<float, float>(prevLoc.X, nextLoc.X, delta);
		float Y = FMath::Lerp<float, float>(prevLoc.Y, nextLoc.Y, delta);
		SetActorLocation(FVector{X, Y, GetActorLocation().Z });

		float scaleX = FMath::Lerp<float, float>(currScale.X, nextScale.X, delta);
		float scaleY = FMath::Lerp<float, float>(currScale.Y, nextScale.Y, delta);
		SetActorScale3D(FVector(scaleX, scaleY, GetActorScale3D().Z));
		
		if (currMoveTime >= maxMoveTime)
		{
			float finalX = FMath::Lerp<float, float>(prevLoc.X, nextLoc.X, 1.f);
			float finalY = FMath::Lerp<float, float>(prevLoc.Y, nextLoc.Y, 1.f);
			SetActorLocation(FVector{ finalX, finalY, GetActorLocation().Z });

			float fScaleX = FMath::Lerp<float, float>(currScale.X, nextScale.X, 1.f);
			float fScaleY = FMath::Lerp<float, float>(currScale.Y, nextScale.Y, 1.f);
			SetActorScale3D(FVector(fScaleX, fScaleY, GetActorScale3D().Z));

			bMove = false;
			currMoveTime = 0.f;
		}
	}
	else
	{
		currWaitTime += DeltaTime;
		if (currWaitTime >= maxWaitTime)
		{
			if (CanMove())
			{
				bMove = true;
				currWaitTime = 0.f;
				StartMove();
			}
		}
	}
}

bool AZoneActor::CanMove() const
{
	return index + 1 < Nodes.Num();
}

void AZoneActor::StartMove()
{
	index++;
	nextLoc = Nodes[index]->GetActorLocation();
	prevLoc = GetActorLocation();

	currScale = GetActorScale3D();
	nextScale = currScale - diffScale;
}


