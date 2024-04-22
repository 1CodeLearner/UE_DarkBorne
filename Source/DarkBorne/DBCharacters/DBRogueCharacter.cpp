// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include "DBCharacterSkill/DBCharacterSkillComponent.h"
#include "DBCharacterSkill/DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "DBCharacterAttack/DBRogueAttackComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>



ADBRogueCharacter::ADBRogueCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �޽� ��ġ ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Ca);

	//SpringArm ������Ʈ ����
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm �� RootComponent �� �ڽ� (���� ������Ʈ�� ����Ʈ �⺻ �ڽ���)
	springArm->SetupAttachment(RootComponent);
	//springArm ��ġ�� �ٲ���
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	//springArm ���� ����
	springArm->SetRelativeRotation(FRotator(0, 0, 0));
	springArm->TargetArmLength = 200;
	springArm->ProbeChannel = ECollisionChannel::ECC_Visibility;
	
	// camera setting
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//camera �� springArm �� �ڽ����� ����
	camera->SetupAttachment(springArm);
	//camera->SetupAttachment(GetMesh(), FName(TEXT("headSocket")));
	camera->SetRelativeLocation(FVector(0, 0, 0));
	camera->SetRelativeRotation(FRotator(-20, 0, 0));
	//(X = -10.260604, Y = 50.000000, Z = 61.809221)
	//(Pitch = -20.000000, Yaw = 0.000000, Roll = 0.000000)
	RogueWeaponComp = CreateDefaultSubobject<UDBRogueWeaponComponent>(TEXT("WeaponComp"));
	RogueWeaponComp->SetupAttachment(GetMesh(), FName(TEXT("RightHandWeapon")));
	RogueWeaponComp->SetRelativeLocation(FVector(-10, 3, 0));

	RogueSkillComponent = CreateDefaultSubobject<UDBRogueSkillComponent>(TEXT("RogueSkillComp"));
	RogueAttackComponent = CreateDefaultSubobject<UDBRogueAttackComponent>(TEXT("RogueAttackComp"));

	
}

void ADBRogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//get materials
	MatArr = GetMesh()->GetMaterials();
	
	MaxHP = 100;
	CurrHP = MaxHP;
}

void ADBRogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeathProcess();
}

void ADBRogueCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		RogueSkillComponent->SetupPlayerInputComponent(enhancedInputComponent);
		RogueAttackComponent->SetupPlayerInputComponent(enhancedInputComponent);
		RogueWeaponComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void ADBRogueCharacter::DeathProcess()
{
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(GetMesh()->GetAnimInstance());

	if (CurrHP <= 0)
	{
		MyCharacterAnim->isDeath = true;
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
	}
}

void ADBRogueCharacter::CurrHPProcess()
{
	
}

