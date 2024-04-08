// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/StaticMeshComponent.h"

ADBItem::ADBItem()
{
	PrimaryActorTick.bCanEverTick = true;
	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
}

void ADBItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADBItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

