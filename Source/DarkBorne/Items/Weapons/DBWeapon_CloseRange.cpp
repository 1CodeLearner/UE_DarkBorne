// Fill out your copyright notice in the Description page of Project Settings.


#include "DBWeapon_CloseRange.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include "../../DBAnimInstance/DBRogueAnimInstance.h"
#include "../../DBCharacters/DBRogueCharacter.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>

ADBWeapon_CloseRange::ADBWeapon_CloseRange()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(SMComp);
	CapsuleComp->SetRelativeLocation(FVector(0, 0, 25));
	CapsuleComp->SetCapsuleHalfHeight(16);
	CapsuleComp->SetCapsuleRadius(3);
	CapsuleComp->SetCollisionProfileName(TEXT("WeaponCapColl"));
}

void ADBWeapon_CloseRange::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Owner in CloseWeapon: %s"), *GetNameSafe(GetOwner()));
	//�������� �浹������ �ϰ�ʹٸ� ���⼭���� �պ���
	if (GetOwner<ACharacter>()->IsLocallyControlled()) 
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ADBWeapon_CloseRange::OnOverlapBegin);
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ADBWeapon_CloseRange::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

// �������� ������ ó��
void ADBWeapon_CloseRange::ServerRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
{
	//���� �ƴ� �ٸ� �α� �÷��̾ otherActor�� ĳ����
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);

	FString Level = GetWorld()->GetMapName();
	Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (Level != TEXT("Level_Lobby"))
	{
		//�÷��̾��� ���� ü�¿��� ���ⵥ������ŭ �������� �ش�
		OtherPlayer->CurrHP = OtherPlayer->CurrHP - WeaponDamage;
		//onRep �Լ��� Ŭ�󿡼��� ȣ��Ǿ ���������� �ѹ� ȣ��������Ѵ�
		OtherPlayer->OnRep_CurrHP();
		UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
			GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);
	}

	MultiRPC_OnOverlapBegin(OtherActor);
}

// Ŭ�󿡼� �浹ó��...
void ADBWeapon_CloseRange::MultiRPC_OnOverlapBegin_Implementation(class AActor* OtherActor)
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


