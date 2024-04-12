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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	// 로그 무기 장착 Scene Comp
	UPROPERTY(EditAnywhere)
	class UDBRogueWeaponComponent* RogueWeaponComp;

	// 로그 스킬 Comp
	UPROPERTY(EditAnywhere)
	class UDBRogueSkillComponent* RogueSkillComponent;
	
	UPROPERTY(EditAnywhere)
	class UDBRogueAttackComponent* RogueAttackComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> MatArr;

public:
	float MaxHP = 100;
	float CurrHP;

};
