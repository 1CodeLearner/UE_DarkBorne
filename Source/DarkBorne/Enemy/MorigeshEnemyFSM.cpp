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
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"

UMorigeshEnemyFSM::UMorigeshEnemyFSM()
{

}

void UMorigeshEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 체크
	//! 나중에 게임 인스턴스로 옮길것
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADBCharacter::StaticClass(), FoundActors);
	enemyTargetList = FoundActors;

	//나 자신 액터
	myActor = Cast<AMorigeshEnemy>(GetOwner());
	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();

	//애님 인스턴스
	anim = Cast<UAnimMorigeshEnemy>(animInstance);

	//공격속도 늦추기
	anim->attackAnimation->RateScale = 0.15f;

	//! 차후를 위한 AI 컨트롤러 및 기타
	ai = Cast<AAIController>(myActor->GetController());
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);





}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isFSMDebugMode)
	{
		UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
		if (enumPtr != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Now State %s"),
				*enumPtr->GetNameStringByIndex((int32)currState));
		}
	}

	//주변 타겟 체크
	TargetCheck();



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

//트랜지션 과정 + 변환
void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{


	//currState 한번 더 초기화(부모단에서 안되는듯)
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

		//랜덤 앵글
		int32 randAngle = FMath::RandRange(0, 359);
		//Y축 적용
		FRotator rot = FRotator(0, randAngle, 0);
		//전방 벡터
		FVector randDir = UKismetMathLibrary::GetForwardVector(rot);
		//랜덤 거리 300~500
		float randDist = FMath::RandRange(300.0f, 500.0f);

		FVector randPos = myActor->GetActorLocation() + randDir * randDist;

		FVector start = myActor->GetActorLocation();
		FVector end = randPos;

		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(myActor);
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 0.1f, 0, 1.0f);
		//PhysicsBody 기준 
		//Collision 수정 필요 할 시 수정 할 것
		bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_PhysicsBody, param);
		if (hit)
		{
			//맞은 위치 에서 캡슐 컴포넌트 사이즈 /2
			patrolPos =  hitInfo.ImpactPoint - (myActor->GetCapsuleComponent()->GetScaledCapsuleRadius()/2);
		}
		else
		{
			patrolPos = randPos;
		}




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
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		myActor->PlayAnimMontage(montage, 1.0f, TEXT("Death01"));
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
	if (nowTarget == nullptr)
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

	if (tempvector.Size() < 1)
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
	ChangeState(EEnemyState::ATTACK_DELAY);
	

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
	if (nowTarget == nullptr) return false;

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

void UMorigeshEnemyFSM::TargetCheck()
{
	//target 선택
	AActor* selectTarget = nullptr;
	float distance = -1;

	if(enemyTargetList.Num() <= 0) return; 
	for (AActor* tempTarget : enemyTargetList)
	{
		//스캔 타겟 위치
		FVector tempTargetLocation = tempTarget->GetActorLocation();
		//내 위치
		FVector tempMyLocation = myActor->GetActorLocation();
		//방향 만들기
		FVector tempdirection = tempTargetLocation - tempMyLocation;
		//거리 체크
		float tempdistance = FVector::Distance(tempTargetLocation, tempMyLocation);
		
		UE_LOG(LogTemp, Warning, TEXT("TempDistance : %f"), tempdistance);
		UE_LOG(LogTemp, Warning, TEXT("traceRange : %f"), traceRange);
		UE_LOG(LogTemp, Warning, TEXT("distance : %f"), distance);
		
		//추적 위치보다 멀고, 이전에 스캔 했던 거리 보다 작으면 넘기기
		if (tempdistance < traceRange )
		{
			if (tempdistance > distance)
			{
				//만약 맞으면 
				//현재 타겟 지정
				selectTarget = tempTarget;
				//임시용 거리 지정
				distance = tempdistance;

			}
		}

		
	}

	if (selectTarget == nullptr)
	{

		nowTarget = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("NoTarget")));
	}
	else
	{
		nowTarget = selectTarget;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Target On : %s"), nowTarget));
	}
}
