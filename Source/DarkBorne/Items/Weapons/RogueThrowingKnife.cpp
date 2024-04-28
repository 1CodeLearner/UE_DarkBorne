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

ARogueThrowingKnife::ARogueThrowingKnife()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(SMComp);
	CapsuleComp->SetRelativeLocation(FVector(0, 0, 19));
	CapsuleComp->SetCapsuleHalfHeight(10);
	CapsuleComp->SetCapsuleRadius(3);
	CapsuleComp->SetCollisionProfileName(TEXT("WeaponCapColl"));

	projectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	projectileComponent->UpdatedComponent = SceneComp;
	projectileComponent->InitialSpeed = 0;
	projectileComponent->MaxSpeed = 2000;
	projectileComponent->bShouldBounce = false;
	projectileComponent->bRotationFollowsVelocity = true;
	projectileComponent->ProjectileGravityScale = 0;
	
}

void ARogueThrowingKnife::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Owner in thisKnife: %s"), *GetNameSafe(GetOwner()));
	//�������� �浹������ �ϰ�ʹٸ� ���⼭���� �պ���
	//if (GetOwner<ACharacter>()->IsLocallyControlled()) {
		//CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ARogueThrowingKnife::OnOverlapBegin);
		//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
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
	FVector TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation()+ RoguePlayer->GetActorForwardVector() * 50 + RoguePlayer->GetActorRightVector() * KnifeNumber * 50;

	// �� ���� * �����˵� �߾Ӱ��� ���ش�
	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;
	
	FRotator SpringArmRotation = RogueCharacter->ThrowKnifePos->GetComponentRotation();
	APlayerController* playerController = Cast<APlayerController>(RogueCharacter->GetController());
	//playerController->PlayerCameraManager->GetCameraRotation();
	FRotator NewRot = RogueCharacter->ThrowKnifePos->GetComponentRotation();
	// ������ ��ġ ���� 
	SetActorLocationAndRotation(TKPosition, NewRot);
}

void ARogueThrowingKnife::UpdateKnifeSpeed()
{

}
