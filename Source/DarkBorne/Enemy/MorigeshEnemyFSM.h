// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/EnemyFSMBase.h"
#include "MorigeshEnemyFSM.generated.h"

/**
 * 
 */
 
 UENUM(BlueprintType)
enum class EMorigeshAttackType : uint8
{
	ATTACK1,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UMorigeshEnemyFSM : public UEnemyFSMBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMorigeshEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> enemyTarget;

	
	UPROPERTY(EditAnywhere)
	class UAnimMorigeshEnemy* anim;


	

	UPROPERTY(EditAnywhere)
	float traceRange = 500;
	
	UPROPERTY(EditAnywhere)
	float attackRange = 200;

	UPROPERTY(EditAnywhere)
	float moveRange = 1000;

	float currTime = 0;
	
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2;
	
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;

	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;

	float viewAngle = 180;

	FVector originPos;
	FVector patrolPos;




public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeState(EEnemyState s);
	
	void UpdateIdle();
	
	void UpdateMove();
	
	void UpdatePatrol();
	
	void UpdateAttack();
	
	void UpdateAttackDelay();
	
	void UpdateDamaged(float deltaTime);

	void UpdateDie();

	bool IsWaitComplete(float delay);

	bool CanTrace();

};
