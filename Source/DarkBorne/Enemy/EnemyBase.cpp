// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/EnemyBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//생성자 부분 따로 처리 할 것
	/*
	* 1. 메쉬 가져오기 및 회전처리
	* 2. FSM 각자 다 다르므로 FSM 별도 처리
	* 3. 
	*/
	
	//생성자 부분 공통 처리 할 것
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Capsule 컴포넌트 CollisonPreset = EnemyProfile (CPPTPS들고옴)
	// Mesh 컴포넌트 CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	//Auto Possess ai 설정 (spawn, placed 둘다 동작하게)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	//초기 체력 세팅
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
/// 데미지 처리
/// </summary>
/// <param name="damage">데미지 처리 입력</param>
/// <returns>죽었는가 리턴 받아서 FSM에서 애니메이션 처리</returns>
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

