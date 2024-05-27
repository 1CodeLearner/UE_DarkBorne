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
#include "Async/Async.h"


UMorigeshEnemyFSM::UMorigeshEnemyFSM()
{

	/*ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Enemy/Morigesh/BP_AIMorigeshEnemyMontage.BP_AIMorigeshEnemyMontage'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}*/
	// 블루프린트 클래스가 제대로 로드되었는지 확인합니다.
	/*static ConstructorHelpers::FClassFinder<AMorigeshWeapon> WeaponBPClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Enemy/Morigesh/BP_MorigeshDaggerProjectile.BP_MorigeshDaggerProjectile'"));
	if (WeaponBPClass.Succeeded())
	{
		weaponFactory = WeaponBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Blueprint class found and set successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Blueprint class not found."));
	}
	*/


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

	if (Cast<AMorigeshEnemy>(myActor))
	{
		if(Cast<AMorigeshEnemy>(myActor)->SoundBoard != nullptr)
		{ 
			this->SoundBoard = Cast<AMorigeshEnemy>(myActor)->SoundBoard;
		}
	}





	
}

void UMorigeshEnemyFSM::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bIsShuttingDown = true;
	Super::EndPlay(EndPlayReason);

}

void UMorigeshEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsFireTiming)
	{

		FireWeapon(ThrowPos);
		IsFireTiming = false;
	}
	//타이머 on
	//타이머 딜레이 & 중간 방해  체크
	//발사
	//FSM 한번 돌고
	//공격이 안들어 왔을때
	//타이머 동작하여
	//공격


	
}

void UMorigeshEnemyFSM::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(UMorigeshEnemyFSM, anim);
	DOREPLIFETIME(UMorigeshEnemyFSM, montage);
}

//트랜지션 과정 + 변환
void UMorigeshEnemyFSM::ChangeState(EEnemyState s)
{
	Super::ChangeState(s);
	//currState 한번 더 초기화(부모단에서 안되는듯)
	currState = s;

	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}
	if (myActor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Running on the Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Running on the Client"));
	}*/
	
	anim->state = currState;
	currTime = 0;
	//MultiRPC_ChangeState(s);
	if (myActor->HasAuthority())
	{
		
		FVector CapturedThrowPos;

		switch (currState)
		{
			case EEnemyState::ATTACK:
				if (nowTarget == nullptr) break;
				
				CapturedThrowPos = nowTarget->GetActorLocation();
				// 비동기 작업 시작
				
				Async(EAsyncExecution::Thread, [this, CapturedThrowPos]()
					{
						FPlatformProcess::Sleep(0.3f); // 1초 대기

						if (bIsShuttingDown)
						{
							return;
						}

						// 메인 스레드에서 작업 수행
						AsyncTask(ENamedThreads::GameThread, [this, CapturedThrowPos]()
							{
								if (!bIsShuttingDown)
								{
									//UE_LOG(LogTemp, Warning, TEXT("Thread Working"));
									IsFireTiming = true;
									ThrowPos = CapturedThrowPos;
								}
							});
					});
			
				
				
				/*ThrowPos = nowTarget->GetActorLocation();
				
				GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UMorigeshEnemyFSM::OnAttackNotify, 1.0f, true);*/
					
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
			default:
				break;
		}
	}
	
	

}

void UMorigeshEnemyFSM::FireWeapon(FVector targetPos)
{
	FVector spawnForward = (targetPos - myActor->GetActorLocation()).GetSafeNormal();
	FVector spawnUp = (targetPos - myActor->GetActorLocation()).GetSafeNormal();

	// spawnBorder를 벡터 값으로 정의
	spawnBorder = FVector(100.0f, 50.0f, 75.0f); // 원하는 변위 값을 설정

	// 각 방향에 대한 변위를 벡터 값으로 설정
	FVector offset = (myActor->GetActorForwardVector() * spawnBorder.X)
		+ (myActor->GetActorUpVector() * spawnBorder.Z)
		+ (myActor->GetActorRightVector() * spawnBorder.Y);

	FVector spawnPos = myActor->GetActorLocation() + offset;

	// spawnPos가 업데이트된 후 다시 방향 벡터 계산
	spawnForward = (targetPos - spawnPos).GetSafeNormal();
	spawnUp = FVector::UpVector;  // 또는 원하는 Up 방향 벡터 설정

	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	//SpawnParams.Owner = myActor;

	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(spawnForward, spawnUp);
	AMorigeshWeapon* weapon = GetWorld()->SpawnActor<AMorigeshWeapon>(weaponFactory, spawnPos, rot, SpawnParams);
	if (weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon spawned at position: %s"), *weapon->GetActorLocation().ToString());
		//weapon->SetOwner(myActor);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon."));
		UE_LOG(LogTemp, Error, TEXT("Failed Target Position: %s"), *targetPos.ToString());
		UE_LOG(LogTemp, Error, TEXT("Failed Actor Position: %s"), *myActor->GetActorLocation().ToString());
	}

	
	/*
	FVector spawnForward = (targetPos - myActor->GetActorLocation()).GetSafeNormal();
	FVector spawnUp = (targetPos - myActor->GetActorLocation()).GetSafeNormal();

	FVector spawnPos = myActor->GetActorLocation() +(myActor->GetActorForwardVector() * spawnBorder) + (myActor->GetActorUpVector() * spawnBorder);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;

	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(spawnForward, spawnUp);
	AMorigeshWeapon* weapon = GetWorld()->SpawnActor<AMorigeshWeapon>(weaponFactory, spawnPos, rot, SpawnParams);
	if (weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon spawned at position: %s"), *weapon->GetActorLocation().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon."));
		UE_LOG(LogTemp, Error, TEXT("Failed Target Position: %s"), *targetPos.ToString());
		UE_LOG(LogTemp, Error, TEXT("Failed Target Position: %s"), *myActor->GetActorLocation().ToString());
	}
	//FVector tempPos = weapon->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("SpawnWeaponPos : %s"), *tempPos.ToString());
	*/
}
//void UMorigeshEnemyFSM::FireWeapon(FVector targetPos)
//{
//	
//	FVector spawnForward = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();
//	FVector spawnUp = (nowTarget->GetActorLocation() - myActor->GetActorLocation()).GetSafeNormal();
//
//	FVector spawnPos = myActor->GetActorLocation() +(myActor->GetActorForwardVector() * spawnBorder) + (myActor->GetActorUpVector() * spawnBorder);
//	
//	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(spawnForward, spawnUp);
//
//	AMorigeshWeapon* weapon = GetWorld()->SpawnActor<AMorigeshWeapon>(weaponFactory, spawnPos, rot);
//	//FVector tempPos = weapon->GetActorLocation();
//	//UE_LOG(LogTemp, Warning, TEXT("SpawnWeaponPos : %s"), *tempPos.ToString());
//	
//}

void UMorigeshEnemyFSM::OnRep_CurrentState()
{
	Super::OnRep_CurrentState();
	if (anim != nullptr)
	{
		anim->state = currState;

	}
	switch (currState)
	{

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
	}
}

void UMorigeshEnemyFSM::UpdateDamaged(float deltaTime)
{
	Super::UpdateDamaged(deltaTime);

	

	if (!anim->isHitting)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::MOVE);
	}
}

//void UMorigeshEnemyFSM::MultiRPC_ChangeState_Implementation(EEnemyState s)
//{
//	
//	//FString OwnerName = myActor->HasAuthority() ? TEXT("Server") : TEXT("Client");
//	//UE_LOG(LogTemp, Warning, TEXT("SC: %s"), *OwnerName);
//	
//	Super::MultiRPC_ChangeState(s);
//}
//
