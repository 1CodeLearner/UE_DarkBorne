// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshWeapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include "../Status/CharacterStatusComponent.h"
#include "../DBCharacters/DBRogueCharacter.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include "../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"
#include "EnemyBase.h"
#include "AnimEnemyBase.h"
#include "../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
AMorigeshWeapon::AMorigeshWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// �浹ü

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	SetRootComponent(compSphere);
	compSphere->SetSphereRadius(13);
	compSphere->SetRelativeLocation(FVector(10,0,0));
	compSphere->SetCollisionProfileName(TEXT("WeaponCapColl"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);
	compMesh->SetRelativeRotation(FRotator(-90,0,0));
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Throwing.SM_Throwing'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}

	// �߻�ü ������ �����ϴ� ������Ʈ
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	// ������ ������Ʈ ����
	compProjectile->SetUpdatedComponent(compSphere);
	// �ʱ� �ӷ�, �ִ� �ӷ�
	compProjectile->InitialSpeed = 1200;
	compProjectile->MaxSpeed = 3000;
	// ƨ��� �Ұ���
	compProjectile->bShouldBounce = false;
	// �󸶳� �� ƨ��� �Ұ�����

}

// Called when the game starts or when spawned
void AMorigeshWeapon::BeginPlay()
{
	Super::BeginPlay();

	compSphere->OnComponentBeginOverlap.AddDynamic(this,&AMorigeshWeapon::OnOverlapBegin);
	SetLifeSpan(4);
}

// Called every frame
void AMorigeshWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMorigeshWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (myActor == nullptr)
	{
		if (GetOwner() != nullptr)
		{
			myActor = Cast<AActor>(GetOwner());
		}
	}
	// ���� �� �ڽ��� �΋H���ٸ�
	if (OtherActor == myActor)
	{
		return;
	}
	ServerRPC_OnOverlapBegin(OtherActor);
}

void AMorigeshWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMorigeshWeapon, myActor);
}

void AMorigeshWeapon::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{

	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	if (OtherPlayer)
	{
		FString Level = GetWorld()->GetMapName();
		Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (Level != TEXT("ThirdPersonMap"))
		{
			UCharacterStatusComponent* StatusComponent = OtherActor->GetComponentByClass<UCharacterStatusComponent>();
			//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
			//�κ� üũ
			StatusComponent->DamageProcess(weaponDamage, this);
			//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ �������� �ش�
			//onRep �Լ��� Ŭ�󿡼��� ȣ��Ǿ ���������� �ѹ� ȣ��������Ѵ�
			StatusComponent->OnRep_CurrHP();

			//UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
			//	GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);

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
	}
	Destroy();
}

void AMorigeshWeapon::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
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
	compSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMorigeshWeapon::AutoDestroy()
{

}

