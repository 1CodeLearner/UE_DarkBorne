// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

/**
 *
 */
class UPlayerEquipmentComponent;
class UDBEquipmentComponent;
class UBaseInventoryComponent;

class UInventoryGridWidget;
class ULootInventoryComponent;
class ULootEquipmentComponent;

enum class EEntityType : uint8;

UCLASS()
class DARKBORNE_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	void DisplayInventory(bool bEnabled);
	//Dynamically change loot display
	UFUNCTION(BlueprintCallable)
	void InitLootDisplay(AActor* OtherEntity);
	UFUNCTION(BlueprintCallable)
	bool IsLootValid() const;

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryGrid_Widget;
	
	//Owning player's Inventories
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UPlayerEquipmentComponent* PlayerEquipmentComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	UDBEquipmentComponent* EquipmentComp;

	//Other entity's inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	TObjectPtr<UDBEquipmentComponent> EquipmentComp_Loot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	TObjectPtr<UPlayerEquipmentComponent> InventoryComp_Loot;

private:
	bool bLootValid;
	void AssignLootFrom(AActor* OtherEntity);
	bool IsValidForInit(const TArray<UBaseInventoryComponent*>& Inventories) const;
	void ClearLoot();

	bool IsValidNum(const TArray<UBaseInventoryComponent*>& Inventories) const;
	bool IsReadyForAssignment() const;
};
