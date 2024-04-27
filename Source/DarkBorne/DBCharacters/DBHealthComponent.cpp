// Fill out your copyright notice in the Description page of Project Settings.


#include "DBHealthComponent.h"

UDBHealthComponent::UDBHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDBHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UDBHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

