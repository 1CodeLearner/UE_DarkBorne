// Fill out your copyright notice in the Description page of Project Settings.


#include "LootEquipmentComponent.h"



ULootEquipmentComponent::ULootEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void ULootEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULootEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void ULootEquipmentComponent::DisplayLoot(AActor* OtherActor, EEntityType EntityType)
{
}

void ULootEquipmentComponent::Server_CopyItems_Implementation(AActor* OtherActor, EEntityType EntityType)
{
}
