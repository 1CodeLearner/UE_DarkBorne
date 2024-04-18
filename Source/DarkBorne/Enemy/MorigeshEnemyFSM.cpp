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
	
	enemyTarget = FoundActors;
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
	float radianViewAngle = FMath::DegreesToRadians(viewAngle* 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	originPos = myActor->GetActorLocation();



	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start State %s"), 
		*enumPtr->GetNameStringByIndex((int32)currState));
	}*/
	

}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime,  TickType, ThisTickFunction);

	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Now State %s"),
		*enumPtr->GetNameStringByIndex((int32)currState));
	}

	if(nowTarget == nullptr)
	{
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
			return;
		}
		else
		{
			nowTarget = selectTarget;
		}
	}
	
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
	//상태 변경에 대한 디버그 출력
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}

	ai->StopMovement();

	currState = s;
	anim->state = currState;
	currTime = 0;

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
			float randDist = FMath::RandRange(100.0f, 500.0f);
			// 3. 1, 2 의 값을 이용해서 랜덤한 위치를 뽑자
			patrolPos = originPos + randDir * randDist;
			// 그 위치로 이동!
			//ai->MoveToLocation(patrolPos);
		}
			break;
		case EEnemyState::ATTACK:
		{
			//// 킥, 펀치 공격할지 설정
			//int32 rand = FMath::RandRange(0, 1);
			//anim->attackType = (EAttackType)rand;
			anim->attackType = (EMorigeshAttackType)0;
		}
			break;
		case EEnemyState::DAMAGE:
		{
			//!/
			// 원래 Montage써서 노티파이 때마다 공격 방식 전환하는데
			// 우리는 지금은 당장 굳이 필요없음
			// 알파때 추가할 것
			// 
			//	// 1. 랜덤한 값을 뽑는다. (1, 2)
			//	int32 rand = FMath::RandRange(1, 2);
			//	// 2. Damage01, Damage02 란 문자열을 만든다.
			//	FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
			//	// 3. Montage 플레이
			//	myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
			//}
		}
			break;
		case EEnemyState::DIE:
		{
			//상기 동일로 일단 제외
			/*
			// 죽는 애니메이션 플레이
			myActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
			// 충돌 처리 되지 않게 하자		
			myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// myActor->Destroy();
			break;
			*/
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
		// 2. target 위치로 움직이자 (Navigation  기능을 통해서)
		//ai->MoveToLocation(nowTarget->GetActorLocation());
		//// 2. 그 방향으로 움직이자. 
		myActor->AddMovementInput(dir.GetSafeNormal());

		// 3. 플레이어와의 거리가 공격 범위보다 작으면
		float dist = dir.Length();
		if (dist < attackRange)
		{
			// 4. 현재 상태를 ATTACK 로 바꾸자
			ChangeState(EEnemyState::ATTACK);
		}
	}

}

void UMorigeshEnemyFSM::UpdatePatrol()
{
	//동적 네브메쉬 나중에 연구

	// 내 위치와 랜덤하게 뽑힌 위치의 거리를 구한다.
	float dist = FVector::Distance(patrolPos, myActor->GetActorLocation());

	// 그 거리가 0이면
	if (dist < 50)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
	/*UE_LOG(LogTemp, Warning, TEXT("ttt"));
	if (enemyTarget.Num() > 0)
	{

		FVector destination = enemyTarget[0]->GetActorLocation();
		FVector dir = destination - myActor->GetActorLocation();
		ai->MoveToLocation(destination);
	}*/
	
}

void UMorigeshEnemyFSM::UpdateAttack()
{
	ChangeState(EEnemyState::ATTACK_DELAY);
}

void UMorigeshEnemyFSM::UpdateAttackDelay()
{
	float dist = FVector::Distance(nowTarget->GetActorLocation(), myActor->GetActorLocation());
	// 그 거리가 공격범위- > 진짜 공격
	if (dist < attackRange)
	{
		// 3. 공격 상태로 가라
		ChangeState(EEnemyState::ATTACK);
	}
	// 인지범위 -> 이동 
	else if (CanTrace())
	{
		ChangeState(EEnemyState::MOVE);
	}
	// 그 외는 -> 대기
	else
	{
		ChangeState(EEnemyState::IDLE);
	}
	currTime = 0;
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
	if (IsWaitComplete(3))
	{
		myActor->Destroy();
	}
}

bool UMorigeshEnemyFSM::IsWaitComplete(float delay)
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delay)
	{
		return true;
	}

	return false;
}

bool UMorigeshEnemyFSM::CanTrace()
{
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
		if(tempdistance > traceRange && tempdistance > distance) continue;
		
		selectTarget = tempTarget;
		distance = tempdistance;
		direction = tempdirection;
	}
	if (selectTarget == nullptr)
	{
		nowTarget = nullptr;
		return false;
	}
	else
	{
		nowTarget = selectTarget;
	}
	float dotProduct = FVector::DotProduct(direction.GetSafeNormal(), myActor->GetActorForwardVector());
	FVector start = myActor->GetActorLocation();
	FVector end = selectTarget->GetActorLocation();
	FHitResult hitInfo;
	FCollisionQueryParams param;
	param.AddIgnoredActor(myActor);
	
	bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);
	if(hit)
	{
		ADBCharacter* hittarget = Cast<ADBCharacter>(hitInfo.GetActor());
		if(hittarget != nullptr) return true;
	}
	return false;
	

}
