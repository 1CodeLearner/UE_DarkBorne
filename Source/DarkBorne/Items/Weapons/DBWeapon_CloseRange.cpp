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
	//서버에서 충돌판정을 하고싶다면 여기서부터 손보자

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
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ACharacter* OtherPlayer = Cast<ACharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	//UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

	// 캐릭터의 GetOnwer로 인스턴스를 가져와 나의 플레이어 애님 인스턴스로 가져온다
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	//UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OtherActor->GetActorNameOrLabel(), *OtherComp->GetFName().ToString());

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
			//UE_LOG(LogTemp,Warning,TEXT("hitting: %s"), *OtherActor->GetName());
		}
	}
}

// 서버에서 데미지 처리
void ADBWeapon_CloseRange::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{

	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	AEnemyBase* OtherEnemy = Cast<AEnemyBase>(OtherActor);
	if (OtherPlayer|| OtherEnemy)
	{
		FString Level = GetWorld()->GetMapName();
		Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (Level != TEXT("ThirdPersonMap"))
		{
		UCharacterStatusComponent* StatusComponent = OtherActor->GetComponentByClass<UCharacterStatusComponent>();
		//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
		//로비 체크
			StatusComponent->DamageProcess(WeaponDamage,GetOwner());
			//플레이어의 현재 체력에서 무기데미지만큼 데미지를 준다
			//onRep 함수는 클라에서만 호출되어서 서버에서도 한번 호출해줘야한다
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


// 클라에서 충돌처리...
void ADBWeapon_CloseRange::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	

	if (Cast<ADBCharacter>(OtherActor))
	{
		//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
		ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

		// 충돌한 액터의 hitting
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
		//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
		AEnemyBase* OtherPlayer = Cast<AEnemyBase>(OtherActor);
		//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
		UAnimEnemyBase* OtherPlayerAnim = Cast<UAnimEnemyBase>(OtherPlayer->GetMesh()->GetAnimInstance());
		// 충돌한 액터의 hitting
		OtherPlayerAnim->isHitting = true;
		UE_LOG(LogTemp,Warning,TEXT("enemyhit"));
	}
	
	//blood VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation(), OtherActor->GetActorRotation() - GetActorRotation());
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


