// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerContoller = Cast<APlayerController>(GetController());

	if (playerContoller == nullptr) return;
	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());

	//서브시스템을 가져왔다면
	if (subSystem)
	{
		subSystem->AddMappingContext(IMC_MainMenu, 0);
	}

}

// Called every frame
void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(IA_CreateSession, ETriggerEvent::Started, this, &AMainMenuPawn::CreateSession);
		enhancedInputComponent->BindAction(IA_FindSession, ETriggerEvent::Started, this, &AMainMenuPawn::FindSession);
	}

}

void AMainMenuPawn::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateSessionTest"));
}

void AMainMenuPawn::FindSession()
{
	UE_LOG(LogTemp, Warning, TEXT("FindSessionTest"));
}

