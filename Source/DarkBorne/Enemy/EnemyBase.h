// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


UENUM(BlueprintType)
enum class EEnemyAttackType : uint8
{
	MELEE,
	RANGED 
};


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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UFUNCTION(BlueprintCallable)
	void DamageProcess(int32 damage, AActor* attackSource);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyFSMBase* baseFSM;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxHP = 100;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int32 currHP = 0;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	EEnemyAttackType enemyAttackType = EEnemyAttackType::MELEE;

public:
	UFUNCTION()
	void OnRef_CurrHP();

};
