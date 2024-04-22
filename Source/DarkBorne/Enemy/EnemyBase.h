// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class DARKBORNE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UFUNCTION(BlueprintCallable)
	void DamageProcess(int32 damage);
public:
	UPROPERTY(EditAnywhere)
	class UEnemyFSMBase* baseFSM;
	

public:
	UPROPERTY(EditAnywhere)
	int32 maxHP = 100;

	UPROPERTY(EditAnywhere)
	int32 currHP = 0;
	


};
