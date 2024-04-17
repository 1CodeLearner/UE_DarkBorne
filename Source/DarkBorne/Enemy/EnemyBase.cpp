// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/EnemyBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//������ �κ� ���� ó�� �� ��
	/*
	* 1. �޽� �������� �� ȸ��ó��
	* 2. FSM ���� �� �ٸ��Ƿ� FSM ���� ó��
	* 3. 
	*/
	
	//������ �κ� ���� ó�� �� ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Capsule ������Ʈ CollisonPreset = EnemyProfile (CPPTPS����)
	// Mesh ������Ʈ CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	//Auto Possess ai ���� (spawn, placed �Ѵ� �����ϰ�)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	//�ʱ� ü�� ����
	currHP  = maxHP;

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// <summary>
/// ������ ó��
/// </summary>
/// <param name="damage">������ ó�� �Է�</param>
/// <returns>�׾��°� ���� �޾Ƽ� FSM���� �ִϸ��̼� ó��</returns>
bool AEnemyBase::DamageProcess(int32 damage)
{
	currHP -= damage;
	if (currHP >= 0)
	{
		currHP = 0;
		return true;
	}
	else return false;
}

