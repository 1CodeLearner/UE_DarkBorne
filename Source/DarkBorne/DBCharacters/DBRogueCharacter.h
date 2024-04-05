// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBCharacter.h"
#include "DBRogueCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API ADBRogueCharacter : public ADBCharacter
{
	GENERATED_BODY()
	
public:
	ADBRogueCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	


};
