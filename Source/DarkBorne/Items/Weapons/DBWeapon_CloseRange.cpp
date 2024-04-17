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
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &ADBWeapon_CloseRange::OnOverlapEnd);
}

void ADBWeapon_CloseRange::BeginPlay()
{

}

void ADBWeapon_CloseRange::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(OtherActor);

	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

	// ���� �� �ڽ��� �΋H���ٸ�
	if (OtherActor == GetOwner())
	{
		
	}
	// ���� �� �ڽ��� �ƴ� ���Ͱ� �΋H���ٸ�
	else if (OtherActor != GetOwner())
	{
		
		//Attack anim notify Start
		RogueAnim->AnimNotify_Start_Damage();

		// �������̸�
		if (RogueAnim->isAttacking)
		{	
			//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ ������
			RoguePlayer->CurrHP = RoguePlayer->CurrHP - WeaponDamage;
			UE_LOG(LogTemp, Warning, TEXT("%.f"), RoguePlayer->CurrHP);
			
		}
	}
}

void ADBWeapon_CloseRange::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(RoguePlayer->GetMesh()->GetAnimInstance());

	if (OtherActor == GetOwner())
	{

	}
	else if (OtherActor != GetOwner())
	{
		
		//Attack Anim Notify End
		RogueAnim->AnimNotify_End_Damage();

		
	}
}
