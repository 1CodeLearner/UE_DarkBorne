// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_CloseRange.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include "../../DBCharacters/DBRogueCharacter.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <DarkBorne/Framework/DBDropItemManager.h>
#include "DarkBorne/TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "DarkBorne/Status/CharacterStatusComponent.h"
#include "DarkBorne/Enemy/EnemyBase.h"
#include "DarkBorne/Enemy/AnimEnemyBase.h"
#include "../../DBCharacters/DBCharacterSkill/DBRogueSkillComponent.h"

ADBWeapon_CloseRange::ADBWeapon_CloseRange()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(SMComp);
	CapsuleComp->SetCollisionProfileName(TEXT("WeaponCapColl"));
}

void ADBWeapon_CloseRange::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Owner in CloseWeapon: %s"), *GetNameSafe(GetOwner()));
	//�������� �浹������ �ϰ�ʹٸ� ���⼭���� �պ���

	if(!GetOwner()) return;

	if (GetOwner<ACharacter>()->IsLocallyControlled())
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ADBWeapon_CloseRange::OnOverlapBegin);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ADBWeapon_CloseRange::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp,Warning,TEXT("HERE2"));
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ACharacter* OtherPlayer = Cast<ACharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	//UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

	// ĳ������ GetOnwer�� �ν��Ͻ��� ������ ���� �÷��̾� �ִ� �ν��Ͻ��� �����´�
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetFName().ToString());

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
			//UE_LOG(LogTemp,Warning,TEXT("hitting: %s"), *OtherActor->GetName());
		}
	}
}

// �������� ������ ó��
void ADBWeapon_CloseRange::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{

	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	AEnemyBase* OtherEnemy = Cast<AEnemyBase>(OtherActor);
	if (OtherPlayer|| OtherEnemy)
	{
		FString Level = GetWorld()->GetMapName();
		Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (Level != TEXT("ThirdPersonMap"))
		{
		UCharacterStatusComponent* StatusComponent = OtherActor->GetComponentByClass<UCharacterStatusComponent>();
		//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
		//�κ� üũ
			StatusComponent->DamageProcess(WeaponDamage,GetOwner());
			//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ �������� �ش�
			//onRep �Լ��� Ŭ�󿡼��� ȣ��Ǿ ���������� �ѹ� ȣ��������Ѵ�
			StatusComponent->OnRep_CurrHP();

			//UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
			//	GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);

			auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
			if (ensure(GM) && StatusComponent->CurrHP <= 0.f && OtherPlayer!= nullptr)
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
}


// Ŭ�󿡼� �浹ó��...
void ADBWeapon_CloseRange::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
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
		UE_LOG(LogTemp,Warning,TEXT("enemyhit"));
	}
	
	//blood VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), OtherActor->GetActorRotation() - GetActorRotation());
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


