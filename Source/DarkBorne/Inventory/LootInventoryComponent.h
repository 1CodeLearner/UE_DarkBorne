// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerEquipmentComponent.h"
#include "LootInventoryComponent.generated.h"

enum class EEntityType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API ULootInventoryComponent : public UPlayerEquipmentComponent
{
	GENERATED_BODY()

public:	
	ULootInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CopyItems(UPlayerEquipmentComponent* Inventory, EEntityType EntityType);

protected:	
	
		
};
