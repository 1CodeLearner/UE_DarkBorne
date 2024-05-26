// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySoundComponent.generated.h"



UENUM()
enum class EInvenEquipType
{
	None UMETA(DisplayName="None"),
	Equip UMETA(DisplayName="Equip"),
	Inventory UMETA(DisplayName="Inventory"),
	Spawn UMETA(DisplayName="Spawn")
};

class UItemObject;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UInventorySoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventorySoundComponent();
	
	void DeclareSoundType(EInvenEquipType Type, UItemObject* ItemObject);

};
