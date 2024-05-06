// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

/**
 *
 */

class UInventoryGridWidget;
class UEquipmentGridWidget;

class UPlayerEquipmentComponent;
class UDBEquipmentComponent;
class UBaseInventoryComponent;
enum class EEntityType : uint8;

UCLASS()
class DARKBORNE_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void DisplayInventory(bool bEnabled);
	//Dynamically change loot display
	UFUNCTION(BlueprintCallable)
	void InitLootDisplay(AActor* OtherEntity);
	UFUNCTION(BlueprintCallable)
	bool IsLootValid() const;

protected:
	//Owning Player's Widgets
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* WBP_InventoryGrid;

	//Owning player's Inventories
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UPlayerEquipmentComponent* PlayerEquipmentComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	UDBEquipmentComponent* EquipmentComp;
	
	//Other entity's Widgets
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Player;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Other;

	//Other entity's inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	TObjectPtr<UDBEquipmentComponent> EquipmentComp_Loot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	TObjectPtr<UPlayerEquipmentComponent> InventoryComp_Loot;
	
	void DisplayPlayerLoot(UPlayerEquipmentComponent* _InventoryComp, UDBEquipmentComponent* _EquipmentComp);
	void DisplayOtherLoot(UPlayerEquipmentComponent* _InventoryComp);
	void HideLoots();

private:
	bool bLootValid;
	void AssignLootFrom(AActor* OtherEntity);
	bool IsValidForInit(const TArray<UBaseInventoryComponent*>& Inventories) const;
		bool IsValidNum(const TArray<UBaseInventoryComponent*>& Inventories) const;
		bool IsReadyForAssignment() const;
	void ClearLoot();
};
