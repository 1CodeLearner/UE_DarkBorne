// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshEnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "../Enemy/EnemyBase.h"
#include "../Enemy/MorigeshEnemy.h"
#include "../Enemy/EnemyFSMBase.h"
#include "../Enemy/AnimMorigeshEnemy.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../DBCharacters/DBCharacter.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

UMorigeshEnemyFSM::UMorigeshEnemyFSM()
{

}

void UMorigeshEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	//!
	//플레이어 리스트
	//나중에 GameInstance에서 관리 하여야 함
	TArray<AActor*> FoundActors;
	//중간에 난입하지 않는 이상 무관할듯
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADBCharacter::StaticClass(), FoundActors);

	//타겟 지정
	enemyTarget = FoundActors;
	//나 지정
	myActor = Cast<AMorigeshEnemy>(GetOwner());

	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	//Morigesh형태


	anim = Cast<UAnimMorigeshEnemy>(animInstance);
	// !
	// 일단 가지고만 있고 나중에 동적 네브 메쉬 깔아줘야함
	//331쪽
	ai = Cast<AAIController>(myActor->GetController());


	//다시 볼것
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	originPos = myActor->GetActorLocation();

	anim->attackAnimation->RateScale = 0.3f;

	/*UCharacterMovementComponent* myMovementComponent;
	myMovementComponent = myActor->GetCharacterMovement();
	myMovementComponent->MaxCustomMovementSpeed = 1024;*/

}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Now State %s"),
			*enumPtr->GetNameStringByIndex((int32)currState));
	}

	
	
	//target 선택
	AActor* selectTarget = nullptr;
	FVector direction;
	float distance = -1;

	for (AActor* tempTarget : enemyTarget)
	{
		FVector tempTargetLocation = tempTarget->GetActorLocation();
		FVector tempMyLocation = myActor->GetActorLocation();
		FVector tempdirection = tempTargetLocation - tempMyLocation;
		float tempdistance = tempdirection.Size();
		if (tempdistance > traceRange && tempdistance > distance) continue;

		selectTarget = tempTarget;
		distance = tempdistance;
		direction = tempdirection;
	}

	if (selectTarget == nullptr)
	{
		
		nowTarget = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("1111")));
	}
	else
	{
		nowTarget = selectTarget;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("2222")));
	}
	

	//예제에 nowtarget == nullptr때 리턴한건 이미 정해져 있어서 의미 없ㅇ므

	switch (currState)
	{
		//중립기어
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

//트랜지션 과정 + 변환
void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{
	

	
	//! 
	//! 
	//ai->StopMovement();
	currState = s;
	anim->state = currState;
	currTime = 0;

	//사전 세팅
	switch (currState)
	{
	case EEnemyState::IDLE:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::PATROL:
	{
		// originPos 기준으로 반경 500 cm 안의 랜덤한 위치를 뽑아서 그 위치로 이동하게한다.
	// 1. 랜덤한 방향을 뽑자

		
		int32 randAngle = FMath::RandRange(0, 359);
		FRotator rot = FRotator(0, randAngle, 0);
		FVector randDir = UKismetMathLibrary::GetForwardVector(rot);
		// 2. 그 방향으로 랜덤한 거리를 뽑자
		float randDist = FMath::RandRange(300.0f, 500.0f);
		// 3. 1, 2 의 값을 이용해서 랜덤한 위치를 뽑자
		patrolPos = myActor->GetActorLocation() + randDir * randDist;

		//UE_LOG(LogTemp, Warning, TEXT("My patrolPos: %f, %f, %f"), patrolPos.X, patrolPos.Y, patrolPos.Z);



		// 그 위치로 이동!
		//ai->MoveToLocation(patrolPos);
	}
	break;
	case EEnemyState::ATTACK:
	{
		//// 킥, 펀치 공격할지 설정
		//int32 rand = FMath::RandRange(0, 1);
		//anim->attackType = (EAttackType)rand;
		
		//콜라이더 켜기
	}
	break;
	case EEnemyState::DAMAGE:
	{
		//1. 랜덤한 값을 뽑는다. (1, 2)
		int32 rand = FMath::RandRange(1, 4);
		//2. Damage01, Damage02 란 문자열을 만든다.
		FString sectionName2 = FString::Printf(TEXT("Damage0%d"), rand);
		//3. Montage 플레이
		myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName2));
		
	}
	break;
	case EEnemyState::DIE:
	{
		myActor->PlayAnimMontage(montage, 1.0f, TEXT("Death01"));
		//상기 동일로 일단 제외
		/*
		// 죽는 애니메이션 플레이
		
		// 충돌 처리 되지 않게 하자
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// myActor->Destroy();
		break;
		*/
		
		SetComponentTickEnabled(false);
	}
	break;
	default:
		break;

	}


}

void UMorigeshEnemyFSM::UpdateIdle()
{
	// 만약에 플레이어를 쫓아갈 수 있다면
	if (CanTrace())
	{
		// 상태를 Move 로 바꿔라
		ChangeState(EEnemyState::MOVE);
	}
	// 그렇지 않고 idleDelayTime 을 지났다면
	else if (IsWaitComplete(idleDelayTime))
	{
		// 상태를 PATROL 로 바꿔라
		ChangeState(EEnemyState::PATROL);
	}
}

void UMorigeshEnemyFSM::UpdateMove()
{
	if(nowTarget == nullptr)
	{
		ChangeState(EEnemyState::IDLE);
		return;
	}
	// 1. 플레이어를 향하는 방향을 구하자
	FVector dir = nowTarget->GetActorLocation() - myActor->GetActorLocation();

	// 처음 위치와 나의 위치의 거리
	float distance = FVector::Distance(nowTarget->GetActorLocation(), myActor->GetActorLocation());
	// 만약에 distance 가 moveRange 보다 커지면 
	if (distance > moveRange)
	{
		// Return 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
	else
	{
		float dist = dir.Length();
		if (dist < attackRange)
		{
			if (IsWaitComplete(preAttackDelayTime))
			{
				ChangeState(EEnemyState::ATTACK);
			}
		}
		else
		{
			myActor->AddMovementInput(dir.GetSafeNormal());
		}
	}

}

void UMorigeshEnemyFSM::UpdatePatrol()
{


	
	FVector tempvector = patrolPos - myActor->GetActorLocation();

	//tempvector.Normalize();
	
	if (tempvector.Size() < 1 )
	{
		ChangeState(EEnemyState::IDLE);
	}
	else
	{
		myActor->AddMovementInput(tempvector.GetSafeNormal());
	}

	

	////동적 네브메쉬 나중에 연구
	//// 내 위치와 랜덤하게 뽑힌 위치의 거리를 구한다.
	//float dist = FVector::Distance(nowTarget->GetActorLocation(), myActor->GetActorLocation());

	//// 그 거리가 0이면
	//if (dist < 50)
	//{
	//	// IDLE 상태로 전환
	//	ChangeState(EEnemyState::IDLE);
	//}

}

void UMorigeshEnemyFSM::UpdateAttack()
{
	anim->attackType = (EMorigeshAttackType)0;
	float time = anim->attackAnimation->GetPlayLength()* anim->attackAnimation->RateScale;
	UE_LOG(LogTemp, Warning,TEXT("time %f"),time);
	if (IsWaitComplete(time))
	{
		
		ChangeState(EEnemyState::ATTACK_DELAY);
	}
		
}



void UMorigeshEnemyFSM::UpdateAttackDelay()
{
	
	
	// 그 거리가 공격범위- > 진짜 공격
	if (IsWaitComplete(attackDelayTime))
	{
		currTime = 0;
		if (nowTarget == nullptr)
		{
			if (CanTrace())
			{
				ChangeState(EEnemyState::MOVE);
			}
			// 그 외는 -> 대기
			else
			{
				ChangeState(EEnemyState::IDLE);
			}
		}
		else
		{
			float dist = FVector::Distance(nowTarget->GetActorLocation(), myActor->GetActorLocation());
			if (dist > attackRange)
			{
				// 3. 공격 상태로 가라
				ChangeState(EEnemyState::MOVE);
			}
			else
			{
				ChangeState(EEnemyState::ATTACK);
			}
		}
		
	}
}

void UMorigeshEnemyFSM::UpdateDamaged(float deltaTime)
{
	if (IsWaitComplete(damageDelayTime))
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}

}

void UMorigeshEnemyFSM::UpdateDie()
{
	
	/*if (IsWaitComplete(3))
	{
		myActor->Destroy();
	}*/
}

bool UMorigeshEnemyFSM::IsWaitComplete(float delay)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("nowcurrTime %f"), currTime));
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delay)
	{
		return true;

	}
	return false;
}

bool UMorigeshEnemyFSM::CanTrace()
{	
	//nowTarget은 계속 초반에 추적중
	if(nowTarget == nullptr) return false;
	
	FVector tempVector = nowTarget->GetActorLocation() - myActor->GetActorLocation();

	float dotProduct = FVector::DotProduct(tempVector.GetSafeNormal(), myActor->GetActorForwardVector());
	FVector start = myActor->GetActorLocation();
	FVector end = nowTarget->GetActorLocation();
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(myActor);
	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 0.1f, 0, 1.0f);
	
	bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_PhysicsBody, param);
	if (hit)
	{
		UE_LOG(LogTemp, Warning, TEXT("2"));
		ADBCharacter* hittarget = Cast<ADBCharacter>(hitInfo.GetActor());
		if (hittarget != nullptr) return true;
	}
	return false;


}