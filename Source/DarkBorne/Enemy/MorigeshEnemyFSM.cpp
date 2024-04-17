// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemy/EnemyBase.h"
#include "../Enemy/MorigeshEnemy.h"
#include "../Enemy/EnemyFSMBase.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>

UMorigeshEnemyFSM::UMorigeshEnemyFSM()
{
	
}

void UMorigeshEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 리스트
	//나중에 GameInstance에서 관리 하여야 함
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), FoundActors);
	
	enemyTarget = FoundActors;
	myActor = Cast<AMorigeshEnemy>(GetOwner());

	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	//Morigesh형태

	anim = Cast<UAnimMorigeshEnemy>(animInstance);
	ai = Cast<AAIController>(myActor->GetController());
	//다시 볼것
	float radianViewAngle = FMath::DegreesToRadians(viewAngle* 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	originPos = myActor->GetActorLocation();



	
}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,  TickType, ThisTickFunction);

	if(target == nullptr) return;
	
	switch (currState)
	{
		case EEnemyState::IDLE:
			UpdateIdle();
			break;
		case EEnemyState::MOVE:
			UpdateMove();
			break;
		case EEnemyState::PATROL:
			UpdatePatrol();
			break;
		case EEnemyState::ATTACK:
			UpdateAttack();
			break;
		case EEnemyState::ATTACK_DELAY:
			UpdateAttackDelay();
			break;
		case EEnemyState::DAMAGE:
			UpdateDamaged(DeltaTime);
			break;
		case EEnemyState::DIE:
			UpdateDie();
			break;
		default:
			break;
	}
}

void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{
	//상태 변경에 대한 디버그 출력
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}
	
	ai->StopMovement();

}

void UMorigeshEnemyFSM::UpdateIdle()
{

}

void UMorigeshEnemyFSM::UpdateMove()
{

}

void UMorigeshEnemyFSM::UpdatePatrol()
{

}

void UMorigeshEnemyFSM::UpdateAttack()
{

}

void UMorigeshEnemyFSM::UpdateAttackDelay()
{

}

void UMorigeshEnemyFSM::UpdateDamaged(float deltaTime)
{

}

void UMorigeshEnemyFSM::UpdateDie()
{

}

bool UMorigeshEnemyFSM::IsWaitComplete(float delay)
{
	return false;
}

bool UMorigeshEnemyFSM::CanTrace()
{
	return false;
}
