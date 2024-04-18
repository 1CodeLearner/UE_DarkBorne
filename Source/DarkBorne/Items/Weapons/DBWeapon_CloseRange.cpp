// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_CloseRange.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include "../../DBCharacters/DBRogueCharacter.h"

ADBWeapon_CloseRange::ADBWeapon_CloseRange()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetRelativeLocation(FVector(0, 0, 25));
	CapsuleComp->SetCapsuleHalfHeight(16);
	CapsuleComp->SetCapsuleRadius(3);

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ADBWeapon_CloseRange::OnOverlapBegin);
}

void ADBWeapon_CloseRange::BeginPlay()
{
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADBWeapon_CloseRange::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));

	// 캐릭터의 GetOnwer로 인스턴스를 가져와 나의 플레이어 애님 인스턴스로 가져온다
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	// 만약 내 자신이 부딫혔다면
	if (OtherActor == GetOwner())
	{
		
	}
	// 만약 내 자신이 아닌 액터가 부딫혔다면
	else if (OtherActor != GetOwner())
	{
		// 공격중이면
		if (MyCharacterAnim->isAttacking)
		{	
			//플레이어의 현재 체력에서 무기데미지만큼 데미지를 준다
			OtherPlayer->CurrHP = OtherPlayer->CurrHP - WeaponDamage;
			UE_LOG(LogTemp, Warning, TEXT("%.f"), OtherPlayer->CurrHP);
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
	}
}
