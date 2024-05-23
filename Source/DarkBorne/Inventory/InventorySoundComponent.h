// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySoundComponent.generated.h"

USTRUCT()
struct FInvenSound
{
	GENERATED_BODY()
public:
	FInvenSound() 
	{
		EquipSound = nullptr;
		InventorySound = nullptr;
		bPlaySound = false;
	}

	UPROPERTY()
	TObjectPtr<USoundBase> EquipSound;
	
	UPROPERTY()
	TObjectPtr<USoundBase> InventorySound;

	UPROPERTY()
	bool bPlaySound;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UInventorySoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventorySoundComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = "Onrep_InvenSound")
	FInvenSound InvenSound;
	UFUNCTION()
	void Onrep_InvenSound();
};
