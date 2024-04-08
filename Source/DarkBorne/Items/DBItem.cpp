// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/StaticMeshComponent.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include "GameFramework/Character.h"

ADBItem::ADBItem()
{
	PrimaryActorTick.bCanEverTick = true;
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
	RootComponent = SMComp;
}

bool ADBItem::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	auto AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && ensureAlways(AnimMontage)) {
		float result = AnimInstance->Montage_Play(AnimMontage);

		if (result == 0.f)
			return false;
		
		if (!SectionName.IsNone())
			AnimInstance->Montage_JumpToSection(SectionName);
		
		return true;
	}
	else
		return false;
}

void ADBItem::BeginPlay()
{
	Super::BeginPlay();

	float what = FMath::RandRange(0.f, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("Prob: %f"), what);
}

void ADBItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

