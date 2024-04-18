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
	//�÷��̾� ����Ʈ
	//���߿� GameInstance���� ���� �Ͽ��� ��
	TArray<AActor*> FoundActors;
	//�߰��� �������� �ʴ� �̻� �����ҵ�
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADBCharacter::StaticClass(), FoundActors);
	
	enemyTarget = FoundActors;
	myActor = Cast<AMorigeshEnemy>(GetOwner());

	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	//Morigesh����

	anim = Cast<UAnimMorigeshEnemy>(animInstance);
	// !
	// �ϴ� ������ �ְ� ���߿� ���� �׺� �޽� ��������
	//331��
	ai = Cast<AAIController>(myActor->GetController());

	//�ٽ� ����
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
		//target ����
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
		//�߸����
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

//Ʈ������ ���� + ��ȯ
void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{
	//���� ���濡 ���� ����� ���
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
			// originPos �������� �ݰ� 500 cm ���� ������ ��ġ�� �̾Ƽ� �� ��ġ�� �̵��ϰ��Ѵ�.
		// 1. ������ ������ ����
			int32 randAngle = FMath::RandRange(0, 359);
			FRotator rot = FRotator(0, randAngle, 0);
			FVector randDir = UKismetMathLibrary::GetForwardVector(rot);
			// 2. �� �������� ������ �Ÿ��� ����
			float randDist = FMath::RandRange(100.0f, 500.0f);
			// 3. 1, 2 �� ���� �̿��ؼ� ������ ��ġ�� ����
			patrolPos = originPos + randDir * randDist;
			// �� ��ġ�� �̵�!
			//ai->MoveToLocation(patrolPos);
		}
			break;
		case EEnemyState::ATTACK:
		{
			//// ű, ��ġ �������� ����
			//int32 rand = FMath::RandRange(0, 1);
			//anim->attackType = (EAttackType)rand;
			anim->attackType = (EMorigeshAttackType)0;
		}
			break;
		case EEnemyState::DAMAGE:
		{
			//!/
			// ���� Montage�Ἥ ��Ƽ���� ������ ���� ��� ��ȯ�ϴµ�
			// �츮�� ������ ���� ���� �ʿ����
			// ���Ķ� �߰��� ��
			// 
			//	// 1. ������ ���� �̴´�. (1, 2)
			//	int32 rand = FMath::RandRange(1, 2);
			//	// 2. Damage01, Damage02 �� ���ڿ��� �����.
			//	FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
			//	// 3. Montage �÷���
			//	myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
			//}
		}
			break;
		case EEnemyState::DIE:
		{
			//��� ���Ϸ� �ϴ� ����
			/*
			// �״� �ִϸ��̼� �÷���
			myActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
			// �浹 ó�� ���� �ʰ� ����		
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
	// ���࿡ �÷��̾ �Ѿư� �� �ִٸ�
	if (CanTrace())
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

void UMorigeshEnemyFSM::UpdateMove()
{
	// 1. �÷��̾ ���ϴ� ������ ������
	FVector dir = nowTarget->GetActorLocation() - myActor->GetActorLocation();

	// ó�� ��ġ�� ���� ��ġ�� �Ÿ�
	float distance = FVector::Distance(nowTarget->GetActorLocation(), myActor->GetActorLocation());
	// ���࿡ distance �� moveRange ���� Ŀ���� 
	if (distance > moveRange)
	{
		// Return ���·� ��ȯ
		ChangeState(EEnemyState::IDLE);
	}
	else
	{
		// 2. target ��ġ�� �������� (Navigation  ����� ���ؼ�)
		//ai->MoveToLocation(nowTarget->GetActorLocation());
		//// 2. �� �������� ��������. 
		myActor->AddMovementInput(dir.GetSafeNormal());

		// 3. �÷��̾���� �Ÿ��� ���� �������� ������
		float dist = dir.Length();
		if (dist < attackRange)
		{
			// 4. ���� ���¸� ATTACK �� �ٲ���
			ChangeState(EEnemyState::ATTACK);
		}
	}

}

void UMorigeshEnemyFSM::UpdatePatrol()
{
	//���� �׺�޽� ���߿� ����

	// �� ��ġ�� �����ϰ� ���� ��ġ�� �Ÿ��� ���Ѵ�.
	float dist = FVector::Distance(patrolPos, myActor->GetActorLocation());

	// �� �Ÿ��� 0�̸�
	if (dist < 50)
	{
		// IDLE ���·� ��ȯ
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
	// �� �Ÿ��� ���ݹ���- > ��¥ ����
	if (dist < attackRange)
	{
		// 3. ���� ���·� ����
		ChangeState(EEnemyState::ATTACK);
	}
	// �������� -> �̵� 
	else if (CanTrace())
	{
		ChangeState(EEnemyState::MOVE);
	}
	// �� �ܴ� -> ���
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
		// IDLE ���·� ��ȯ
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
	//target ����
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
