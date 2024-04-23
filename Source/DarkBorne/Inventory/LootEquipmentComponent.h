// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBEquipmentComponent.h"
#include "LootEquipmentComponent.generated.h"

/**
 *
 */
UCLASS()
class DARKBORNE_API ULootEquipmentComponent : public UDBEquipmentComponent
{
public:
	ULootEquipmentComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	GENERATED_BODY()
	UFUNCTION(BlueprintCallable)
	void DisplayLoot(AActor* OtherActor, EEntityType EntityType);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_CopyItems(AActor* OtherActor, EEntityType EntityType);
};
