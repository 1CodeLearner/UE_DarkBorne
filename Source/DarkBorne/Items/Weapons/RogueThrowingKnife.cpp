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
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "DarkBorne/Status/CharacterStatusComponent.h"
#include "DarkBorne/Enemy/EnemyBase.h"
#include "DarkBorne/Enemy/AnimEnemyBase.h"
#include "DarkBorne/TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/TimelineComponent.h>

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

	ThrowKnifeVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Aura"));
	ThrowKnifeVFX->SetupAttachment(SMComp);
}

void ARogueThrowingKnife::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Owner in thisKnife: %s"), *GetNameSafe(GetOwner()));
	//�������� �浹������ �ϰ�ʹٸ� ���⼭���� �պ���
	if (!GetOwner()) return;

	if (GetOwner<ACharacter>()->IsLocallyControlled())
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ARogueThrowingKnife::OnOverlapBegin);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//Timeline
	//Ŀ�갡 �ִٸ�
	if (KnifeCurve)
	{	
		if (GetOwner<ACharacter>()->IsLocallyControlled())
		{
			Server_Timeline(timelineOffset);
		}
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
		UpdateKnifeLocation(DeltaTime);
		//Timeline�� �ð��� tick������ �޴´�
		CurveTimeline.TickTimeline(DeltaTime);
	}
}

void ARogueThrowingKnife::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �߾ӽ��̶� ������ źâ replicated
	DOREPLIFETIME(ARogueThrowingKnife, halfValue);
	DOREPLIFETIME(ARogueThrowingKnife, KnifeNumber);
	DOREPLIFETIME(ARogueThrowingKnife, isThrowing);
	DOREPLIFETIME(ARogueThrowingKnife, timelineOffset);
}

void ARogueThrowingKnife::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	
	// ĳ������ GetOnwer�� �ν��Ͻ��� ������ ���� �÷��̾� �ִ� �ν��Ͻ��� �����´�
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	//UE_LOG(LogTemp, Warning, TEXT("--- %s %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetFName().ToString());

	// ���� �� �ڽ��� �΋H���ٸ�
	if (OtherActor == GetOwner())
	{
		return;
	}

	// ���� �� �ڽ��� �ƴ� ���Ͱ� �΋H���ٸ�
	if (OtherActor != GetOwner())
	{
		// �������̸�
		if (MyCharacterAnim->isAttacking)
		{
			ServerRPC_OnOverlapBegin(OtherActor);
		}
		ServerRPC_WallOnOnerlapBegin(OtherActor);	
	}
}

void ARogueThrowingKnife::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	AEnemyBase* OtherEnemy = Cast<AEnemyBase>(OtherActor);

	if(OtherPlayer|| OtherEnemy)
	{
		FString Level = GetWorld()->GetMapName();
		Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		if (Level != TEXT("Level_Lobby"))
		{
			UCharacterStatusComponent* StatusComponent = OtherActor->GetComponentByClass<UCharacterStatusComponent>();

			StatusComponent->DamageProcess(WeaponDamage, GetOwner());
			//StatusComponent->OnRep_CurrHP();
			auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
			if (ensure(GM) && StatusComponent->CurrHP <= 0.f && OtherPlayer != nullptr)
			{
				auto PC = OtherPlayer->GetOwner<APlayerController>();
				if (PC)
				{
					GM->OnPlayerDead(PC);
				}
			}
		}
	MultiRPC_OnOverlapBegin(OtherActor);
	Destroy();
	}
}

void ARogueThrowingKnife::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{	
	if (Cast<ADBCharacter>(OtherActor))
	{
		//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
		ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

		// �浹�� ������ hitting
		OtherPlayerAnim->isHitting = true;
		if (OtherPlayerAnim->isHitting)
		{
			// �ٸ� ĳ���Ͱ� ���Ż����϶� �¾Ҵٸ� ���� Ǯ���ֱ�
			UDBRogueSkillComponent* RogueSkillComponent = OtherPlayer->GetComponentByClass<UDBRogueSkillComponent>();
			if (RogueSkillComponent->isVanish)
			{
				RogueSkillComponent->DeactiveRogueQSkill();
			}
		}
	}
	else if (Cast<AEnemyBase>(OtherActor))
	{
		//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
		AEnemyBase* OtherPlayer = Cast<AEnemyBase>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UAnimEnemyBase* OtherPlayerAnim = Cast<UAnimEnemyBase>(OtherPlayer->GetMesh()->GetAnimInstance());
		// �浹�� ������ hitting
		OtherPlayerAnim->isHitting = true;
	}
	//blood VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), OtherActor->GetActorRotation() - GetActorRotation());
	//CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ARogueThrowingKnife::ServerRPC_WallOnOnerlapBegin_Implementation(class AActor* OtherActor)
{
	MultiRPC_WallOnOverlapBegin(OtherActor);
}

void ARogueThrowingKnife::MultiRPC_WallOnOverlapBegin_Implementation(class AActor* OtherActor)
{
	 if (Cast<AActor>(OtherActor))
	{
		 //blood VFX
			 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitWallVFX, GetActorLocation(), OtherActor->GetActorRotation() - GetActorRotation());
		 Destroy();
	}
}

void ARogueThrowingKnife::UpdateKnifeLocation(float DeltaTime)
{
	//SkillComp���� for������ ������Ų �����˵��� ƽ���� ��� ��ġ �����ش�

	// �������� ���� ���
	AActor* RoguePlayer = GetOwner();
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(RoguePlayer);

	// ������ ��ġ�� �÷��̾� ��ġ + �÷��̾� �� ���� * 50 / ������ i ���� 50��ŭ �߰���Ű��
	TKPosition = RogueCharacter->ThrowKnifePos->GetComponentLocation() +
		RogueCharacter->GetActorForwardVector() +
		RogueCharacter->GetActorRightVector() * KnifeNumber * 50;
	// �� ���� * �����˵� �߾Ӱ��� ���ش�
	TKPosition -= RoguePlayer->GetActorRightVector() * halfValue;

	// TKPos���� ������ �̵��ϴ� �Ÿ�
	TKEndPos = RogueCharacter->ThrowKnifePos->GetComponentLocation() +
		RogueCharacter->GetActorForwardVector() +
		RogueCharacter->GetActorRightVector() * KnifeNumber * 50;

	TKEndPos -= RoguePlayer->GetActorRightVector() * halfValue;
	TKEndPos.Z += ZOffset;

	TKFirstRotation = RogueCharacter->ThrowKnifePos->GetUpVector().Rotation();
	TKFirstRotation.Normalize();
	SetActorRotation(TKFirstRotation);
}

//Timeline
void ARogueThrowingKnife::TimelineProgress(float value)
{
	// ���� : value�� Float curveŬ������ ��
	FVector NewNewLocation = FMath::Lerp(TKPosition, TKEndPos, value);
	SetActorLocation(NewNewLocation);
}

void ARogueThrowingKnife::Server_Timeline_Implementation(float timelineOff)
{
	// timelineOffset : ������ �ð����� �����´�
	timelineOffset = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
	// �����÷��̾ ���� ȣ�� (����RPC���� ȣ���������)
	OnRep_Timeline();
}


void ARogueThrowingKnife::OnRep_Timeline()
{
	//Ÿ�Ӷ����� Ư�� ����Ʈ���� float �� ���ŵɶ� ȣ��Ǵ� ��������Ʈ
	FOnTimelineFloat TimelineProgress;

	//TimelineProgress ��������Ʈ ���ε�
	TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

	//AddInterpFloat : Ÿ�Ӷ��ο� ���� ���� �Լ� �߰�
	CurveTimeline.AddInterpFloat(KnifeCurve, TimelineProgress);
	CurveTimeline.SetLooping(true);
	CurveTimeline.PlayFromStart();

	// ������ ��ŸƮ �������� �����Ѵ�.
	CurveTimeline.SetPlaybackPosition(timelineOffset, false);
}

// Ŭ�󿡼� ���� ���� ��ġ ���
void ARogueThrowingKnife::MultiRPC_RogueThrowKnifeAttack_Implementation(bool isLineHit, FRotator EndRotation, FVector startPos)
{
	//AActor* RoguePlayer = GetOwner();
	ADBRogueCharacter* RogueCharacter = Cast<ADBRogueCharacter>(GetOwner());

	// Ʈ���̽� �¾Ҵٸ�
	if (isLineHit)
	{
		SetActorLocation(startPos);
		SetActorRotation(EndRotation);
	}
	else
	{
		// õ�忡 �¾����� ���� ī�޶��� �������� ��������
		FRotator TKRotation = RogueCharacter->camera->GetForwardVector().Rotation();
		TKRotation.Normalize();
		SetActorRotation(TKRotation);
	}
	isThrowing = true;
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	projectileComponent->ProjectileGravityScale = 0.0f;
	projectileComponent->SetActive(true, true);
	projectileComponent->SetVelocityInLocalSpace(FVector(3000, 0, 0));
	PlayMontage(RogueCharacter, FName("ESkill_Start"));
	SetLifeSpan(3);
}


