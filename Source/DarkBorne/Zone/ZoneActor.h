// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneActor.generated.h"


class AZoneNode;

UCLASS()
class DARKBORNE_API AZoneActor : public AActor
{
	GENERATED_BODY()

public:
	AZoneActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
	TObjectPtr<AZoneNode> Next;

private:

	bool CanMove() const;
	void StartMove();

	FVector prevLoc;
	FVector nextLoc;

	float totalMoveTime;
	float maxMoveTime;
	
	bool bMove;

	float totalWaitTime;
	float maxWaitTime;

};
