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
	//�������� �浹������ �ϰ�ʹٸ� ���⼭���� �պ���
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

	// ���� ��Ŭ���� �����ٸ�
	// �������� updateKnifeLocation�� ��Ȱ��ȭ ��Ű��
	// Ŭ���ϸ� isThrowing�� true��
	if (!isThrowing)
	{
		//��ġ ����
		UpdateKnifeLocation();
	}
	
}

void ARogueThrowingKnife::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �߾ӽ��̶� ������ źâ replicated
	DOREPLIFETIME(ARogueThrowingKnife, halfValue);
	DOREPLIFETIME(ARogueThrowingKnife, KnifeNumber);
	DOREPLIFETIME(ARogueThrowingKnife, isThrowing);
	 
}

void ARogueThrowingKnife::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

	// ĳ������ GetOnwer�� �ν��Ͻ��� ������ ���� �÷��̾� �ִ� �ν��Ͻ��� �����´�
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetFName().ToString());

	// ���� �� �ڽ��� �΋H���ٸ�
	if (OtherActor == GetOwner())
	{
		return;
	}
	// ���� �� �ڽ��� �ƴ� ���Ͱ� �΋H���ٸ�
	else if (OtherActor != GetOwner())
	{
		// �������̸�
		if (MyCharacterAnim->isAttacking)
		{
			ServerRPC_OnOverlapBegin(OtherActor);
			
		}
	}
}

void ARogueThrowingKnife::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);

	//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ �������� �ش�
	OtherPlayer->CurrHP = OtherPlayer->CurrHP - WeaponDamage;
	//onRep �Լ��� Ŭ�󿡼��� ȣ��Ǿ ���������� �ѹ� ȣ��������Ѵ�
	OtherPlayer->OnRep_CurrHP();

	UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
		GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);

	MultiRPC_OnOverlapBegin(OtherActor);
	Destroy();
}

void ARogueThrowingKnife::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());
	// ĳ������ GetOnwer�� �ν��Ͻ��� ������ ���� �÷��̾� �ִ� �ν��Ͻ��� �����´�
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	// �浹�� ������ hitting
	OtherPlayerAnim->isHitting = true;
	//blood VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), OtherPlayer->GetActorRotation() - GetActorRotation());
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ARogueThrowingKnife::UpdateKnifeLocation()
{
	//SkillComp���� for������ ������Ų �����˵��� ƽ���� ��� ��ġ �����ش�

	// �������� ���� ���
	AActor* RoguePlayer = GetOwner();
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);

	// ������ ��ġ�� �÷��̾� ��ġ + �÷��̾� �� ���� * 50 / ������ i ���� 50��ŭ �߰���Ű��
	FVector TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation() + RoguePlayer->GetActorForwardVector() * 50 + RoguePlayer->GetActorRightVector() * KnifeNumber * 50;
	// �� ���� * �����˵� �߾Ӱ��� ���ش�
	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;

	FRotator TKRotation = RogueCharacter->ThrowKnifePos->GetForwardVector().Rotation();
	TKRotation.Normalize();

	SetActorLocationAndRotation(TKPosition, TKRotation);
}

// �������� ��ġ ����
//void ARogueThrowingKnife::ServerRPC_UpdateKnifeLocation_Implementation()
//{
//	//SkillComp���� for������ ������Ų �����˵��� ƽ���� ��� ��ġ �����ش�
//
//	// �������� ���� ���
//	AActor* RoguePlayer = GetOwner();
//	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);
//
//	// ������ ��ġ�� �÷��̾� ��ġ + �÷��̾� �� ���� * 50 / ������ i ���� 50��ŭ �߰���Ű��
//	FVector TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation() + RoguePlayer->GetActorForwardVector() * 50 + RoguePlayer->GetActorRightVector//() * KnifeNumber * 50;
//	// �� ���� * �����˵� �߾Ӱ��� ���ش�
//	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;
//
//	FRotator NewRot = RogueCharacter->ThrowKnifePos->GetForwardVector().Rotation();
//	NewRot.Normalize();
//	
//	MultiRPC_UpdateKnifeLocation(TKPosition, NewRot);
//}
//
//// �������� ������ ��ġ�� Ŭ��� �ѷ��ش�
//void ARogueThrowingKnife::MultiRPC_UpdateKnifeLocation_Implementation(FVector TKPosition, FRotator NewRot)
//{
//	// ������ ��ġ ���� 
//	SetActorLocationAndRotation(TKPosition, NewRot);
//}

// Ŭ�󿡼� ���� ���� ��ġ ���
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

