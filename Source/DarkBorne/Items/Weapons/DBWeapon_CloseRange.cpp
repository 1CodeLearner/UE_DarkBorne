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
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));

	// ĳ������ GetOnwer�� �ν��Ͻ��� ������ ���� �÷��̾� �ִ� �ν��Ͻ��� �����´�
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance());

	// ���� �� �ڽ��� �΋H���ٸ�
	if (OtherActor == GetOwner())
	{
		
	}
	// ���� �� �ڽ��� �ƴ� ���Ͱ� �΋H���ٸ�
	else if (OtherActor != GetOwner())
	{
		// �������̸�
		if (MyCharacterAnim->isAttacking)
		{	
			//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ �������� �ش�
			OtherPlayer->CurrHP = OtherPlayer->CurrHP - WeaponDamage;
			UE_LOG(LogTemp, Warning, TEXT("%.f"), OtherPlayer->CurrHP);
			CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
	}
}
