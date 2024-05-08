// Fill out your copyright notice in the Description page of Project Settings.


#include "LootInventoryComponent.h"
#include "../Framework/EntityTypes.h"
#include "../DBCharacters/DBCharacter.h"
#include "InventoryMainWidget.h"

ULootInventoryComponent::ULootInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULootInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void ULootInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
} 
