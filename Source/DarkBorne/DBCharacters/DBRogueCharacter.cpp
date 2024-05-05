// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include "DBCharacterSkill/DBRogueSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "DBCharacterAttack/DBRogueAttackComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/SpringArmComponent.h>
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include "../Inventory/DBEquipmentComponent.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ArrowComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "../Framework/DBPlayerController.h"



ADBRogueCharacter::ADBRogueCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//SpringArm 컴포넌트 생성
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	//springArm 을 RootComponent 의 자식 (루프 컴포넌트는 디폴트 기본 자식임)
	springArm->SetupAttachment(RootComponent);
	//springArm 위치를 바꾸자
	springArm->SetRelativeLocation(FVector(0, 0, 0));
	//springArm 각도 변경
	springArm->SetRelativeRotation(FRotator(0, 0, 0));
	springArm->TargetArmLength = 200;
	springArm->ProbeChannel = ECollisionChannel::ECC_Visibility;
	springArm->bUsePawnControlRotation = true;
	springArm->SocketOffset = FVector(0, 0, 150);

	// camera setting
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//camera 를 springArm 의 자식으로 셋팅
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

	ThrowKnifePos = CreateDefaultSubobject<UArrowComponent>(TEXT("ThrowKnifePos"));
	ThrowKnifePos->SetupAttachment(GetMesh());
	
	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 500.f;

	
}

void ADBRogueCharacter::BeginPlay()
{
	Super::BeginPlay();

	//get materials
	MatArr = GetMesh()->GetMaterials();

	MaxHP = 100;
	CurrHP = MaxHP;
	// 시작 시 현재 hp 
	OnRep_CurrHP();

	//beginPlay때 컴포넌트 가져오고
	//RogueSkillComponent = GetComponentByClass<UDBRogueSkillComponent>();
	
	//input component를 캐스팅해서 가져온다음
	//UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	//component 붙이기
	//RogueSkillComponent->SetupPlayerInputComponent(enhancedInputComponent);
	
}


void ADBRogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//서버면
	if (HasAuthority())
	{
		DeathProcess();

		knifePos = ThrowKnifePos->GetComponentLocation();
		knifeRot = ThrowKnifePos->GetComponentRotation();

	}
	else
	{
		//내 것이라면
		if (IsLocallyControlled() == false)
		{
			ThrowKnifePos->SetWorldLocation(knifePos);
			ThrowKnifePos->SetWorldRotation(knifeRot);
		}

	}
}

void ADBRogueCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		

		//RogueSkillComponent->SetupPlayerInputComponent(enhancedInputComponent);
		RogueSkillComponent->SetupPlayerInputComponent(enhancedInputComponent);
		RogueAttackComponent->SetupPlayerInputComponent(enhancedInputComponent);
		RogueWeaponComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void ADBRogueCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBRogueCharacter, knifePos);
	DOREPLIFETIME(ADBRogueCharacter, knifeRot);
}

void ADBRogueCharacter::DeathProcess()
{
	ServerRPC_DeathProcess();
}

void ADBRogueCharacter::ServerRPC_DeathProcess_Implementation()
{
	MultiRPC_DeathProcess();
}

void ADBRogueCharacter::MultiRPC_DeathProcess_Implementation()
{
	UDBRogueAnimInstance* MyCharacterAnim = Cast<UDBRogueAnimInstance>(GetMesh()->GetAnimInstance());
	if (CurrHP <= 0 && !MyCharacterAnim->isDeath)
	{
		MyCharacterAnim->isDeath = true;
		UE_LOG(LogTemp, Warning, TEXT("%s"), GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();
		bUseControllerRotationYaw = false;

		if (IsLocallyControlled())
		{
			ADBPlayerController* pc = Cast<ADBPlayerController>(GetWorld()->GetFirstPlayerController());
			//APlayerController* pc = GetWorld()->GetFirstPlayerController();
			//pc->SetShowMouseCursor(true);
			DisableInput(pc);
			
			pc->ChangeToSpectator();
			//pc->SetInputMode(FInputModeGameOnly());
		}
	}
}


