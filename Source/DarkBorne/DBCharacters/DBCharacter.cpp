// Fill out your copyright notice in the Description page of Project Settings.


#include "DBCharacter.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include "DBCharacterSkill/DBCharacterSkillComponent.h"
#include "DBCharacterAttack/DBCharacterAttackComponent.h"


// Sets default values
ADBCharacter::ADBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();
	AActor* actor = GetOwner();
	
	if (IsLocallyControlled())
	{
		//get APlayerController
		APlayerController* playerContoller = Cast<APlayerController>(GetController());

		if(playerContoller == nullptr) return;
		//get subSystem
		UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());

		//서브시스템을 가져왔다면
		if (subSystem)
		{
			subSystem->AddMappingContext(imc_DBMapping, 0);
		}
	}
	
	
}

// Called every frame
void ADBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_DB_Move, ETriggerEvent::Triggered, this, &ADBCharacter::EnhancedMove);
		enhancedInputComponent->BindAction(ia_DB_Jump, ETriggerEvent::Triggered, this, &ADBCharacter::EnhancedJump);
		enhancedInputComponent->BindAction(ia_DB_Look, ETriggerEvent::Triggered, this, &ADBCharacter::EnhancedLook);
		

		
	}
}



void ADBCharacter::EnhancedMove(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();
	FVector originVec = FVector(dir.Y, dir.X, 0);
	FVector newVec = GetTransform().TransformVector(originVec);

	AddMovementInput(newVec);
}

void ADBCharacter::EnhancedJump(const struct FInputActionValue& value)
{
	Jump();

}

void ADBCharacter::EnhancedLook(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();

	AddControllerYawInput(dir.X);
	AddControllerPitchInput(dir.Y);
}

