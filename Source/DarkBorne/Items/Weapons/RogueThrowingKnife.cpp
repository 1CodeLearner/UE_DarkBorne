// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueThrowingKnife.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "../../DBCharacters/DBRogueCharacter.h"
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ArrowComponent.h>
#include <Net/UnrealNetwork.h>
#include "../../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>

ARogueThrowingKnife::ARogueThrowingKnife()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(SMComp);
	CapsuleComp->SetRelativeLocation(FVector(0, 0, 19));
	CapsuleComp->SetCapsuleHalfHeight(22);
	CapsuleComp->SetCapsuleRadius(3);
	CapsuleComp->SetCollisionProfileName(TEXT("WeaponCapColl"));

	projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	projectileComponent->UpdatedComponent = SceneComp;
	projectileComponent->InitialSpeed = 0;
	projectileComponent->MaxSpeed = 3000;
	projectileComponent->bShouldBounce = false;
	projectileComponent->bRotationFollowsVelocity = true;
	projectileComponent->ProjectileGravityScale = 0;
	projectileComponent->bAutoActivate = false;
	projectileComponent->bInterpMovement = true;
	projectileComponent->bInterpRotation = true;

	ThrowKnifeTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	ThrowKnifeTrail->SetupAttachment(SMComp);
}

void ARogueThrowingKnife::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Owner in thisKnife: %s"), *GetNameSafe(GetOwner()));
	//서버에서 충돌판정을 하고싶다면 여기서부터 손보자
	if (!GetOwner()) return;
	
	if (GetOwner<ACharacter>()->IsLocallyControlled()) 
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ARogueThrowingKnife::OnOverlapBegin);
		//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARogueThrowingKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 만약 좌클릭을 눌렀다면
	// 수리검의 updateKnifeLocation을 비활성화 시키자
	// 클릭하면 isThrowing을 true로
	if (!isThrowing)
	{
		//위치 갱신
		UpdateKnifeLocation();
	}
	
}

void ARogueThrowingKnife::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 중앙식이랑 수리검 탄창 replicated
	DOREPLIFETIME(ARogueThrowingKnife, halfValue);
	DOREPLIFETIME(ARogueThrowingKnife, KnifeNumber);
	DOREPLIFETIME(ARogueThrowingKnife, isThrowing);
	 
}

void ARogueThrowingKnife::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

	// 캐릭터의 GetOnwer로 인스턴스를 가져와 나의 플레이어 애님 인스턴스로 가져온다
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetFName().ToString());

	// 만약 내 자신이 부딫혔다면
	if (OtherActor == GetOwner())
	{
		return;
	}
	// 만약 내 자신이 아닌 액터가 부딫혔다면
	else if (OtherActor != GetOwner())
	{
		// 공격중이면
		if (MyCharacterAnim->isAttacking)
		{
			ServerRPC_OnOverlapBegin(OtherActor);
			
		}
	}
}

void ARogueThrowingKnife::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);

	//플레이어의 현재 체력에서 무기데미지만큼 데미지를 준다
	OtherPlayer->CurrHP = OtherPlayer->CurrHP - WeaponDamage;
	//onRep 함수는 클라에서만 호출되어서 서버에서도 한번 호출해줘야한다
	OtherPlayer->OnRep_CurrHP();

	UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
		GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);

	MultiRPC_OnOverlapBegin(OtherActor);
	Destroy();
}

void ARogueThrowingKnife::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());
	// 캐릭터의 GetOnwer로 인스턴스를 가져와 나의 플레이어 애님 인스턴스로 가져온다
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	// 충돌한 액터의 hitting
	OtherPlayerAnim->isHitting = true;
	//blood VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), OtherPlayer->GetActorRotation() - GetActorRotation());
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ARogueThrowingKnife::UpdateKnifeLocation()
{
	//SkillComp에서 for문으로 스폰시킨 수리검들을 틱으로 계속 위치 갱신준다

	// 수리검의 오너 담기
	AActor* RoguePlayer = GetOwner();
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);

	// 수리검 위치는 플레이어 위치 + 플레이어 앞 벡터 * 50 / 간격을 i 마다 50만큼 추가시키기
	FVector TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation() + RoguePlayer->GetActorForwardVector() * 50 + RoguePlayer->GetActorRightVector() * KnifeNumber * 50;
	// 옆 벡터 * 수리검들 중앙값을 빼준다
	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;

	FRotator TKRotation = RogueCharacter->ThrowKnifePos->GetForwardVector().Rotation();
	TKRotation.Normalize();

	SetActorLocationAndRotation(TKPosition, TKRotation);
}

// 서버에서 위치 갱신
//void ARogueThrowingKnife::ServerRPC_UpdateKnifeLocation_Implementation()
//{
//	//SkillComp에서 for문으로 스폰시킨 수리검들을 틱으로 계속 위치 갱신준다
//
//	// 수리검의 오너 담기
//	AActor* RoguePlayer = GetOwner();
//	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);
//
//	// 수리검 위치는 플레이어 위치 + 플레이어 앞 벡터 * 50 / 간격을 i 마다 50만큼 추가시키기
//	FVector TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation() + RoguePlayer->GetActorForwardVector() * 50 + RoguePlayer->GetActorRightVector//() * KnifeNumber * 50;
//	// 옆 벡터 * 수리검들 중앙값을 빼준다
//	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;
//
//	FRotator NewRot = RogueCharacter->ThrowKnifePos->GetForwardVector().Rotation();
//	NewRot.Normalize();
//	
//	MultiRPC_UpdateKnifeLocation(TKPosition, NewRot);
//}
//
//// 서버에서 갱신한 위치를 클라로 뿌려준다
//void ARogueThrowingKnife::MultiRPC_UpdateKnifeLocation_Implementation(FVector TKPosition, FRotator NewRot)
//{
//	// 수리검 위치 갱신 
//	SetActorLocationAndRotation(TKPosition, NewRot);
//}

// 클라에서 각자 로컬 위치 계산
void ARogueThrowingKnife::MultiRPC_RogueThrowKnifeAttack_Implementation()
{
	AActor* RoguePlayer = GetOwner();
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);
	UDBRogueSkillComponent* RogueSkillComponent = GetOwner()->GetComponentByClass<UDBRogueSkillComponent>();

	isThrowing = true;
	projectileComponent->ProjectileGravityScale = 0.0f;
	//projectileComponent->InitialSpeed = 3000;
	projectileComponent->SetActive(true, true);
	projectileComponent->SetVelocityInLocalSpace(FVector(3000, 0, 0));
	PlayMontage(RogueCharacter, FName("ESkill_Start"));
	SetLifeSpan(3);
	
}

