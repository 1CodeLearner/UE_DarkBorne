// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/EnemyFSMBase.h"
#include "../Enemy/EnemyBase.h"
#include <Kismet/GameplayStatics.h>
#include <AIController.h>
#include "../DBCharacters/DBCharacter.h"
#include <NavigationSystem.h>
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include <Net/UnrealNetwork.h>
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "AnimEnemyBase.h"
#include "../Status/CharacterStatusComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "AnimMorigeshEnemy.h"
#include "DarkBorne/DBCharacters/DBRogueCharacter.h"
#include "DarkBorne/DBAnimInstance/DBRogueAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// FSM공통 부분 추가 발생시 추가할 스크립트
//당장은 쓰이지 않음

// Sets default values for this component's properties
UEnemyFSMBase::UEnemyFSMBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//컴포넌트 멀티플레이 활성화
	SetIsReplicatedByDefault(true);
	// ...
	// 
	// 

}


// Called when the game starts
void UEnemyFSMBase::BeginPlay()
{
	Super::BeginPlay();

	myActor = Cast<AEnemyBase>(GetOwner());

	if (myActor->HasAuthority())
	{
		AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
		if (GameMode)
		{
			// 캐스팅하여 사용자 정의 게임 모드로 변환
			tpsgamemode = Cast<ATP_ThirdPersonGameMode>(GameMode);
			if (tpsgamemode)
			{
				// 여기에서 MyGameMode의 멤버 함수나 변수 사용 가능
				//ActivePlayers = tpsgamemode->;
				tpsgamemode->OnPlayerUpdate.AddUObject(this, &UEnemyFSMBase::OnPlayerStatus);
				TArray<ADBCharacter*> FoundActors;
				FoundActors = tpsgamemode->GetPlayerCharacters();
				enemyTargetList = FoundActors;
			}
		}



		ai = Cast<AAIController>(myActor->GetController());

		float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
		viewAngle = FMath::Cos(radianViewAngle);

		switch (myActor->enemyAttackType)
		{
		case EEnemyAttackType::MELEE:
			engageRange = closedWeaponAttackRange;
			break;
		case EEnemyAttackType::RANGED:
			engageRange = rangedWeaponAttackRange;
			break;
		}

		ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	}


}


// Called every frame
void UEnemyFSMBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (myActor->HasAuthority())
	{	
		if (IsTargetDeath(nowTarget))
		{
			ChangeState(EEnemyState::IDLE);
		}
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
			EyeOnTarget();
			UpdateAttack();
			break;
		case EEnemyState::ATTACK_DELAY:
			EyeOnTarget();
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

}

void UEnemyFSMBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEnemyFSMBase, currState);
	DOREPLIFETIME(UEnemyFSMBase, enemyTargetList);
}

void UEnemyFSMBase::ChangeState(EEnemyState e)
{



	// 바뀌는 상태를 출력하자
	/*
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)e));
	}
	*/
	
	currState = e;
	//???? ????? ?? ??
	OnRep_CurrentState();
	currTime = 0;
	if (myActor->HasAuthority())
	{
		if (ai == nullptr) return;
		ai->StopMovement();
		switch (currState)
		{
		case EEnemyState::IDLE:

			break;
		case EEnemyState::MOVE:
			if (nowTarget != nullptr)
			{

				FVector pos = nowTarget->GetActorLocation();
				ai->MoveToLocation(pos);
			}
			break;
		case EEnemyState::PATROL:
		{
			bool islooping = true;
			FNavLocation loc;
			while (islooping)
			{
				/**/

				bool result = ns->GetRandomReachablePointInRadius(myActor->GetActorLocation(), randMaxMovementRange, loc);
				islooping = !result;

			}
			patrolPos = loc.Location;
			ai->MoveToLocation(patrolPos);
		}
		break;
		case EEnemyState::ATTACK:
			break;
		case EEnemyState::DAMAGE:
			break;
		case EEnemyState::DIE:
			

			// 위치 고정
			break;
		default:
			break;

		}
	}

}

void UEnemyFSMBase::UpdateIdle()
{

	// 만약에 플레이어를 쫓아갈 수 있다면
	if (IsVisibleCheck())
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

void UEnemyFSMBase::UpdateMove()
{
	if (IsOutRangeCheck())
	{
		ChangeState(EEnemyState::IDLE);
		return;
	}
	else if (IsEngageRangeCheck() && CanVisibleAttack())
	{
		ChangeState(EEnemyState::ATTACK);
	}

}

void UEnemyFSMBase::UpdatePatrol()
{
	if (IsVisibleCheck())
	{
		ChangeState(EEnemyState::MOVE);
	}

	else if (IsPatrolPos())
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}

}

void UEnemyFSMBase::UpdateAttack()
{
		ChangeState(EEnemyState::ATTACK_DELAY);
	
	
}

void UEnemyFSMBase::UpdateAttackDelay()
{
	if (IsWaitComplete(attackDelayTime))
	{
		if (IsTargetDeath(nowTarget))
		{
			ChangeState(EEnemyState::IDLE);
		}
		
		//교전거리 0, 보임 0
		else if (IsEngageRangeCheck() && CanVisibleAttack())
		{
			ChangeState(EEnemyState::ATTACK);
		}
		// 인지범위 -> 이동 
		//교전거리 x, 보임 x
		else if (!IsEngageRangeCheck() || !CanVisibleAttack())
		{
			ChangeState(EEnemyState::MOVE);
		}
		// 그 외는 -> 대기
		
		else 
		{
			ChangeState(EEnemyState::IDLE);
		}

		// 4. 현재시간 초기화
		currTime = 0;
	}
}

void UEnemyFSMBase::UpdateDamaged(float deltaTime)
{
	
	
}

void UEnemyFSMBase::UpdateDie()
{

}

void UEnemyFSMBase::EyeOnTarget()
{
	FVector camForward = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();
	FVector camUp = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(camForward, camUp);
	myActor->SetActorRotation(rot);
}


bool UEnemyFSMBase::IsWaitComplete(float delay)
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("nowcurrTime %f"), currTime));
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delay)
	{
		return true;

	}
	return false;
}


/// <summary>
/// 
/// </summary>
/// <returns>true = 공격 가능 거리,false = 공격 불가능 거리</returns>
bool UEnemyFSMBase::IsEngageRangeCheck()
{
	if (nowTarget == nullptr) return false;

	FVector dir = nowTarget->GetActorLocation() - myActor->GetActorLocation();
	float dist = dir.Length();

	if (dist <= engageRange)
	{
		return true;
	}
	return false;
}

/// <summary>
/// 추적중에 범위 벗어났는지 체크하는 로직
/// </summary>
/// <returns>true면 범위 벗어남, false면 내부</returns>
bool UEnemyFSMBase::IsOutRangeCheck()
{
	if (nowTarget == nullptr) return false;

	FVector dir = nowTarget->GetActorLocation() - myActor->GetActorLocation();
	float dist = dir.Length();

	if (dist > traceOutRange)
	{
		nowTarget = nullptr;
		return true;
	}
	return false;
}

/// <summary>
/// 처음 시야 감지용
/// </summary>
/// <returns></returns>
bool UEnemyFSMBase::IsVisibleCheck()
{
	AActor* selectTarget = nullptr;
	FVector myActorLocation = myActor->GetActorLocation();
	float distance = -1;

	if (enemyTargetList.Num() <= 0) return false;
	for (AActor* tempTarget : enemyTargetList)
	{

		FVector dir = tempTarget->GetActorLocation() - myActor->GetActorLocation();
		float dist = dir.Length();
		float dot = FVector::DotProduct(dir.GetSafeNormal(), myActor->GetActorForwardVector());


		//거리나 시야각 범위 벗어나면 제외
		if (dist > visibleRange || dot < viewAngle) continue;
		//아니라면 Trace 체크
		else
		{
			FVector start = myActor->GetActorLocation();
			FVector end = tempTarget->GetActorLocation();
			FHitResult hitInfo;
			FCollisionQueryParams param;
			param.AddIgnoredActor(myActor);
			bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_PhysicsBody, param);
			if (hit && (Cast<ADBCharacter>(hitInfo.GetActor()) != nullptr))
			{
				if (distance <= 0 || distance > dist)
					selectTarget = tempTarget;
				distance = dist;
			}

		}

	}
	if (selectTarget == nullptr)
	{
		return false;
	}
	else
	{
		nowTarget = selectTarget;
		return true;
	}


}

bool UEnemyFSMBase::CanVisibleAttack()
{
	if (nowTarget == nullptr) return false;

	FVector start = myActor->GetActorLocation();
	FVector end = nowTarget->GetActorLocation();

	FHitResult hitInfo;
	FCollisionQueryParams param;

	param.AddIgnoredActor(myActor);

	bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_PhysicsBody, param);

	if (hit && (Cast<ADBCharacter>(hitInfo.GetActor()) != nullptr))
	{
		return true;
	}
	return false;
}

bool UEnemyFSMBase::IsPatrolPos()
{
	FVector temp = patrolPos;
	temp.Z = myActor->GetActorLocation().Z;
	float dist = FVector::Distance(temp, myActor->GetActorLocation());

	// 그 거리가 0이면
	if (dist < 50)
	{
		return true;
	}
	return false;
}
/*
void UEnemyFSMBase::DeathCheck()
{
	UAnimEnemyBase* OtherPlayerAnim = Cast<UAnimEnemyBase>(myActor->GetMesh()->GetAnimInstance());
	if (myActor->CharacterStatusComponent->CurrHP <= 0 && !OtherPlayerAnim->isDeath)
	{
		OtherPlayerAnim->isDeath = true;
		currState = EEnemyState::DIE;
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		myActor->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		myActor->
	}




}
*/
void UEnemyFSMBase::OnRep_CurrentState()
{
	// 클라이언트에서 상태 변경 시 추가적으로 필요한 동작 구현
	if (myActor != nullptr && currState == EEnemyState::DIE)
	{
		//???? "Item" collision object type ? ??
		myActor->GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		myActor->GetMesh()->SetEnableGravity(false);
		
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//이거 해줘야 바닥 뜷고 안떨어짐
		myActor->GetCharacterMovement()->DisableMovement();
		
		
	}
}

void UEnemyFSMBase::OnPlayerStatus(class ADBPlayerController* temp, bool temp2)
{
	TArray<class ADBCharacter*> list;

	enemyTargetList = tpsgamemode->GetPlayerCharacters();

	//UE_LOG(LogTemp,Warning,TEXT("this"));
	return;
}

bool UEnemyFSMBase::IsTargetDeath(class AActor* target)
{
	if(nowTarget == nullptr) return false;
	if (Cast<ADBCharacter>(target))
	{
		//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
		ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(target);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

		// 충돌한 액터의 hitting
		if (OtherPlayerAnim != nullptr && OtherPlayerAnim->isDeath)
		{
			nowTarget = nullptr;
			return true;
		}
	}
	return false;
}

