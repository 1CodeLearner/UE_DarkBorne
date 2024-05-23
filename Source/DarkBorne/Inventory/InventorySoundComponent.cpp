// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySoundComponent.h"
#include "Net/UnrealNetwork.h"

UInventorySoundComponent::UInventorySoundComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventorySoundComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventorySoundComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventorySoundComponent, InvenSound, COND_OwnerOnly);
}

void UInventorySoundComponent::Onrep_InvenSound()
{
	if (InvenSound.bPlaySound) 
	{
		InvenSound.bPlaySound = false;
		
	}
}


void UInventorySoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

