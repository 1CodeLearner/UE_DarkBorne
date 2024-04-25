// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "J_TesetEnemy.generated.h"

UCLASS()
class DARKBORNE_API AJ_TesetEnemy : public APawn
{
	GENERATED_BODY()

public:
	AJ_TesetEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
  