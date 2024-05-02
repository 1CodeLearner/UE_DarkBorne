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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	//camera
	UPROPERTY(EditAnywhere)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArm;

	// �α� ���� ���� Scene Comp
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDBRogueWeaponComponent* RogueWeaponComp;

	// �α� ��ų Comp
	UPROPERTY(EditAnywhere)
	class UDBRogueSkillComponent* RogueSkillComponent;
	
	UPROPERTY(EditAnywhere)
	class UDBRogueAttackComponent* RogueAttackComponent;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> MatArr;

public:
	UFUNCTION()
	void DeathProcess();

	UFUNCTION(Server, Reliable)
	void ServerRPC_DeathProcess();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DeathProcess();

	UPROPERTY(Replicated)
	FRotator knifeRot;

	UPROPERTY(Replicated)
	FVector knifePos;

};
