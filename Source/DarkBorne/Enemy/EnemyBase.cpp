// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/EnemyBase.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "../Enemy/EnemyFSMBase.h"
#include <Net/UnrealNetwork.h>
#include "GameFramework/Actor.h"
#include "../Status/CharacterStatusComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	//������ �κ� ���� ó�� �� ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->SetNetAddressable();
	GetCharacterMovement()->SetIsReplicated(true);

	//baseFSM = CreateDefaultSubobject<UEnemyFSMBase>(TEXT("BaseFSM"));
	
	
	

	
	// Capsule ������Ʈ CollisonPreset = EnemyProfile (CPPTPS����)
	// Mesh ������Ʈ CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerColl"));
	GetMesh()->SetCollisionProfileName(TEXT("PlayerMeshColl"));
	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>("CharacterStatusComp");
	CharacterStatusComponent->SetNetAddressable();
	CharacterStatusComponent ->SetIsReplicated(true);
	//Auto Possess ai ���� (spawn, placed �Ѵ� �����ϰ�)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FString OwnerName = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	//FString FSMTypeString = EnumToString(TEXT("EVehicleType"), static_cast<int32>(baseFSM->currState));
	//UE_LOG(LogTemp, Warning, TEXT("Owner: %s, fsmStatus: %s"), *OwnerName, FSMTypeString);
	/*
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*OwnerName,
			*enumPtr->GetNameStringByIndex((int32)baseFSM->currState));
	}
	*/
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEnemyBase, currHP);
	DOREPLIFETIME(AEnemyBase, enemyAttackType);
	//DOREPLIFETIME(AEnemyBase, baseFSM);
}

/// <summary>
/// ������ ó�� �� Ÿ�� ���� 
/// </summary>
/// <param name="damage">������ ó�� �Է�</param>
/// <returns>�׾��°� ���� �޾Ƽ� FSM���� �ִϸ��̼� ó��</returns>
void AEnemyBase::DamageProcess(float damage, AActor* attackSource)
{
	
	if (baseFSM != nullptr)
	{
		baseFSM->nowTarget = attackSource;
		baseFSM->ChangeState(EEnemyState::MOVE);
	}
	
	CharacterStatusComponent->CurrHP -= damage;
	

	if (CharacterStatusComponent->CurrHP <= 0)
	{
		CharacterStatusComponent->CurrHP = 0;
		
		baseFSM->ChangeState(EEnemyState::DIE);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("enemy now Health: %f"), CharacterStatusComponent->CurrHP);
		baseFSM->ChangeState(EEnemyState::DAMAGE);
	}
}



