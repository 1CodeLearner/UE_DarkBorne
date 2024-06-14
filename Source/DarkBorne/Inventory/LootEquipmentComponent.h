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
	GENERATED_BODY()

public:
	ULootEquipmentComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DisplayLoot(AActor* OtherActor, EEntityType EntityType);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_CopyItems(AActor* OtherActor, EEntityType EntityType);
};
