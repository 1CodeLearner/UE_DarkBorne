// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DBCharacter.generated.h"


UCLASS()
class DARKBORNE_API ADBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	//player Mapping Context
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_DBMapping;

	//player input
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Jump;
	
public:
	//Default 이동 관련 함수들
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);
};
