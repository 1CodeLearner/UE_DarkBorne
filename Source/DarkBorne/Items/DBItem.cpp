// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/StaticMeshComponent.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include "GameFramework/Character.h"

ADBItem::ADBItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
	SMComp->SetCollisionProfileName(TEXT("WeaponSMColl"));
	SMComp->SetupAttachment(RootComponent);
	
	
	//RootComponent = SMComp;
}

bool ADBItem::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	if(!PlayerCharacter) return false;
	if(!PlayerCharacter->GetMesh()) return false;
	if(!PlayerCharacter->GetMesh()->GetAnimInstance()) return false;

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

}

void ADBItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

