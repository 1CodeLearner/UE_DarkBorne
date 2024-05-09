// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerEquipmentComponent.h"
#include "LootInventoryComponent.generated.h"

class ADBCharacter;

enum class EEntityType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class DARKBORNE_API ULootInventoryComponent : public UPlayerEquipmentComponent
{
	GENERATED_BODY()

public:	
	ULootInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	/// <summary>
	/// Set which items to create and assign to this loot-able inventory using DataTable DT_DropRate.
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	FName DropName;
};
