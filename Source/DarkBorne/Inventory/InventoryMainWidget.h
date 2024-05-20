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
class UItemObject;
enum class EEntityType : uint8;

class UInputMappingContext;

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

	//display loot
	UFUNCTION(BlueprintCallable)
	void InitLootDisplay(AActor* OtherEntity);
	UFUNCTION(BlueprintCallable)
	bool IsLootValid() const;

protected:
	//Owning player's Inventories.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UPlayerEquipmentComponent* PlayerEquipmentComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	UDBEquipmentComponent* EquipmentComp;

	//Owning Player's Widgets.
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* WBP_InventoryGrid; //Already initialized in blueprint
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEquipmentGridWidget* EquipmentGrid_Weapon; //Already initialized in blueprint
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEquipmentGridWidget* EquipmentGrid_Consumable; //Already initialized in blueprint


	/*
	Other Player/Entity's inventory for looting.
	Other player has UDBEquipmentComponent. Entity does not have UDBEquipmentComponent.*/
	UPROPERTY()
	TObjectPtr<UDBEquipmentComponent> EquipmentComp_Loot;
	//Both other player and other entity have UPlayerEquipmentComponent.
	UPROPERTY()
	TObjectPtr<UPlayerEquipmentComponent> InventoryComp_Loot;

	/*
	Other Player/Entity's Widgets for looting.
	Other Player.*/
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Player;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEquipmentGridWidget* EquipmentLoot_Weapon;
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEquipmentGridWidget* EquipmentLoot_Consumable;
	//Other Entity.
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UInventoryGridWidget* InventoryLoot_Other;

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

	//UFUNCTION()
	//void OnItemClicked(UBaseItemWidget* ItemWidgetClicked, EGridWidgetType GridWidgetType, bool bIsRightButton);
};
