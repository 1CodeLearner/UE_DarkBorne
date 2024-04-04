// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/SkeletalMeshComponent.h"

ADBItem::ADBItem()
{
	PrimaryActorTick.bCanEverTick = true;
	SKMComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKMComp"));
}

void ADBItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADBItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

