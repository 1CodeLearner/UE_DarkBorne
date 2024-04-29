// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMBase.generated.h"


//���� Enemy Locomotion ���� ����
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
	CHILD_STATE // �Ļ� Ŭ���� state ���ٿ�
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
	
	
	
	virtual void ChangeState(EEnemyState e);


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

	//�Ÿ� ���� �ִ��� üũ �ؼ� ������ �i�ư� �뵵
	virtual bool IsEngageRangeCheck();
	//�Ÿ� ���� ������ üũ�ؼ� �Ȧi�ư� �뵵
	virtual bool IsOutRangeCheck();
	//ó���� �þ� �� ���ͼ� ���� üũ �� �뵵
	virtual bool IsVisibleCheck();
	//���Ÿ� ��� ���� �ϰԲ� �þ߿� ���̴���
	virtual bool CanVisibleAttack();

	virtual bool IsPatrolPos();

	


public:

	
	//AIController
	UPROPERTY(EditAnywhere)
	class AAIController* ai;


	TArray<AActor*> enemyTargetList;

	EEnemyState currState = EEnemyState::IDLE;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class AActor* nowTarget;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class AEnemyBase* myActor;


	

	//�ð� ����

	//�ð� üũ��
	float currTime = 0;

	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;

	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;

	UPROPERTY(EditAnywhere)
	float preAttackDelayTime = 1;

	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2;


	//�Ÿ� ����
	//ó�� Ž�� ���̴� �Ÿ�
	UPROPERTY(EditAnywhere)
	float visibleRange = 500;

	UPROPERTY(EditAnywhere)
	float engageRange= 100;

	UPROPERTY(EditAnywhere)
	float traceOutRange = 2000;

	//��Ʈ�� �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float randMaxMovementRange = 500;

	//Ž�� �þ�
	float viewAngle = 180;

	FVector patrolPos;

	//���� �Ÿ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rangedWeaponAttackRange = 800;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float closedWeaponAttackRange = 30;


	


		
};
