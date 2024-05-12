// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshEnemyFSM.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/MorigeshEnemy.h"
#include "../Enemy/EnemyFSMBase.h"
#include "../Enemy/AnimMorigeshEnemy.h"
#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>
#include "../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../DBCharacters/DBCharacter.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "../Enemy/MorigeshWeapon.h"
#include <Net/UnrealNetwork.h>


UMorigeshEnemyFSM::UMorigeshEnemyFSM()
{

	/*ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Enemy/Morigesh/BP_AIMorigeshEnemyMontage.BP_AIMorigeshEnemyMontage'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}*/

	


}

void UMorigeshEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 체크
	//! 나중에 게임 인스턴스로 옮길것
	
	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();

	//애님 인스턴스
	anim = Cast<UAnimMorigeshEnemy>(animInstance);




	
}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UMorigeshEnemyFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMorigeshEnemyFSM, anim);
	DOREPLIFETIME(UMorigeshEnemyFSM, montage);
}

//트랜지션 과정 + 변환
void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{
	Super::ChangeState(s);
	//currState 한번 더 초기화(부모단에서 안되는듯)
	currState = s;

	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}
	
	anim->state = currState;
	currTime = 0;

	
	switch (currState)
	{
		case EEnemyState::ATTACK:
			FireWeapon(nowTarget->GetActorLocation());
		break;
		case EEnemyState::DAMAGE:
		{
			//1. 랜덤한 값을 뽑는다. (1, 2)
			int32 rand = FMath::RandRange(1, 4);
			//2. Damage01, Damage02 란 문자열을 만든다.
			FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
			//3. Montage 플레이
			myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
		}
			break;
		//case EEnemyState::DIE:
		//{
		//	// 죽는 애니메이션 플레이
		//	myActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
		//	// 충돌 처리 되지 않게 하자		
		//	myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//}
			break;
		default:
			break;
	}


}

void UMorigeshEnemyFSM::FireWeapon(FVector targetPos)
{
	
	FVector spawnForward = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();
	FVector spawnUp = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();

	FVector spawnPos = myActor->GetActorLocation() +(myActor->GetActorForwardVector() * spawnBorder) + (myActor->GetActorUpVector() * spawnBorder);
	
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(spawnForward, spawnUp);

	AMorigeshWeapon* weapon = GetWorld()->SpawnActor<AMorigeshWeapon>(weaponFactory, spawnPos, rot);
	//FVector tempPos = weapon->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("SpawnWeaponPos : %s"), *tempPos.ToString());
	
}

void UMorigeshEnemyFSM::OnRep_CurrentState()
{
	Super::OnRep_CurrentState();
	if (anim != nullptr)
	{
		anim->state = currState;

	}
}

void UMorigeshEnemyFSM::UpdateDamaged(float deltaTime)
{
	Super::UpdateDamaged(deltaTime);
	if (!anim->isHitting)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
}
