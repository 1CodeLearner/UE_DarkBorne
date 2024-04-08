// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>

ADBRogueCharacter::ADBRogueCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/GhostSamurai_Bundle/GhostSamurai/Character/Mesh/SK_GhostSamurai.SK_GhostSamurai'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	// 메쉬 위치 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(GetMesh(), FName(TEXT("headSocket")));
	camera->SetRelativeLocation(FVector(9.8f, 13.2f, 0));
	camera->SetRelativeRotation(FRotator(0, 80, 270));
}

void ADBRogueCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADBRogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




