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

// FSM���� �κ� �߰� �߻��� �߰��� ��ũ��Ʈ
//������ ������ ����

// Sets default values for this component's properties
UEnemyFSMBase::UEnemyFSMBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//������Ʈ ��Ƽ�÷��� Ȱ��ȭ
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
			// ĳ�����Ͽ� ����� ���� ���� ���� ��ȯ
			tpsgamemode = Cast<ATP_ThirdPersonGameMode>(GameMode);
			if (tpsgamemode)
			{
				// ���⿡�� MyGameMode�� ��� �Լ��� ���� ��� ����
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



	// �ٲ�� ���¸� �������
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
			

			// ��ġ ����
			break;
		default:
			break;

		}
	}

}

void UEnemyFSMBase::UpdateIdle()
{

	// ���࿡ �÷��̾ �Ѿư� �� �ִٸ�
	if (IsVisibleCheck())
	{
		// ���¸� Move �� �ٲ��
		ChangeState(EEnemyState::MOVE);
	}
	// �׷��� �ʰ� idleDelayTime �� �����ٸ�
	else if (IsWaitComplete(idleDelayTime))
	{
		// ���¸� PATROL �� �ٲ��
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
		// IDLE ���·� ��ȯ
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
		
		//�����Ÿ� 0, ���� 0
		else if (IsEngageRangeCheck() && CanVisibleAttack())
		{
			ChangeState(EEnemyState::ATTACK);
		}
		// �������� -> �̵� 
		//�����Ÿ� x, ���� x
		else if (!IsEngageRangeCheck() || !CanVisibleAttack())
		{
			ChangeState(EEnemyState::MOVE);
		}
		// �� �ܴ� -> ���
		
		else 
		{
			ChangeState(EEnemyState::IDLE);
		}

		// 4. ����ð� �ʱ�ȭ
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
/// <returns>true = ���� ���� �Ÿ�,false = ���� �Ұ��� �Ÿ�</returns>
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
/// �����߿� ���� ������� üũ�ϴ� ����
/// </summary>
/// <returns>true�� ���� ���, false�� ����</returns>
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
/// ó�� �þ� ������
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


		//�Ÿ��� �þ߰� ���� ����� ����
		if (dist > visibleRange || dot < viewAngle) continue;
		//�ƴ϶�� Trace üũ
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

	// �� �Ÿ��� 0�̸�
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
	// Ŭ���̾�Ʈ���� ���� ���� �� �߰������� �ʿ��� ���� ����
	if (myActor != nullptr && currState == EEnemyState::DIE)
	{
		//???? "Item" collision object type ? ??
		myActor->GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		myActor->GetMesh()->SetEnableGravity(false);
		
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//�̰� ����� �ٴ� ���� �ȶ�����
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
		//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
		ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(target);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

		// �浹�� ������ hitting
		if (OtherPlayerAnim != nullptr && OtherPlayerAnim->isDeath)
		{
			nowTarget = nullptr;
			return true;
		}
	}
	return false;
}

