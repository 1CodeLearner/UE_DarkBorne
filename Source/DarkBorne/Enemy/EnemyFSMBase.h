// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMBase.generated.h"


//공통 Enemy Locomotion 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	PATROL,
	ATTACK,
	ATTACK_DELAY,
	DAMAGE,
	DIE,
	CHILD_STATE // 파생 클래스 state 접근용
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UEnemyFSMBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSMBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	



public:

	
	virtual void UpdateIdle();

	virtual void UpdateMove();

	virtual void UpdatePatrol();

	virtual void UpdateAttack();

	virtual void UpdateAttackDelay();

	virtual void UpdateDamaged(float deltaTime);

	virtual void UpdateDie();

	void EyeOnTarget();

	virtual bool IsWaitComplete(float delay);

	//거리 내에 있는지 체크 해서 끝까지 쫒아갈 용도
	virtual bool IsEngageRangeCheck();
	//거리 벗어 났는지 체크해서 안쫒아갈 용도
	virtual bool IsOutRangeCheck();
	//
	virtual bool IsTargetSightOut();

	//처음에 시야 에 들어와서 추적 체크 할 용도
	virtual bool IsVisibleCheck();
	//원거리 사격 가능 하게끔 시야에 보이는지
	virtual bool CanVisibleAttack();

	virtual bool IsPatrolPos();
	
	//virtual void DeathCheck();

	UFUNCTION()
	virtual void OnRep_CurrentState();

	void OnPlayerStatus(class ADBPlayerController* temp,bool temp2);

	bool IsTargetDeath(AActor* target);

	UFUNCTION()
	virtual void ChangeState(EEnemyState e);


	
	//virtual void MultiRPC_ChangeState(EEnemyState s) {};


public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ATP_ThirdPersonGameMode* tpsgamemode;

	//AIController
	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<class ADBCharacter*> enemyTargetList;

	//UPROPERTY( EditAnywhere, BlueprintReadWrite)
	//TMap<class ADBPlayerController*, bool>* ActivePlayers;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentState, BlueprintReadWrite, EditAnywhere)
	EEnemyState currState = EEnemyState::IDLE;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class AActor* nowTarget;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class AEnemyBase* myActor;

	UPROPERTY(EditAnywhere)
	class UNavigationSystemV1* ns;

	//시간 관련

	//시간 체크용
	float currTime = 0;

	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;

	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;

	UPROPERTY(EditAnywhere)
	float preAttackDelayTime = 1;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2;


	//거리 관련
	//처음 탐지 보이는 거리
	UPROPERTY(EditAnywhere)
	float visibleRange = 500;

	UPROPERTY(EditAnywhere)
	float engageRange= 100;

	UPROPERTY(EditAnywhere)
	float traceOutRange = 2000;

	//패트롤 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float randMaxMovementRange = 500;

	//탐지 시야
	float viewAngle = 180;

	FVector patrolPos;

	//공격 거리 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rangedWeaponAttackRange = 800;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float closedWeaponAttackRange = 30;


	


		
};
