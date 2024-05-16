// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryComponent.h"
#include "DBEquipmentComponent.generated.h"

class UItemObject;
class UPlayerEquipmentComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DARKBORNE_API UDBEquipmentComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()
	friend class ULootInventoryComponent;
public:
	UDBEquipmentComponent();

	UFUNCTION(BlueprintCallable)
	void AddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer);
	virtual bool TryAddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer) override;
	virtual void RemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiToServer) override;

	UFUNCTION(Server, Reliable)
	void Server_TaxiForAddItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiedInventoryComp);
	UFUNCTION(Server, Reliable)
	void Server_TaxiForRemoveItem(UItemObject* ItemObject, UBaseInventoryComponent* TaxiedInventoryComp);

	UFUNCTION(Server, Reliable)
	void Server_AddItem(UItemObject* ItemObject);
	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject) override;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemObject*> GetSlots() const;

	UFUNCTION(BlueprintCallable)
	UItemObject* GetSlotItem(ESlotType SlotType) const;

	UFUNCTION(BlueprintCallable)
	bool IsSlotVacant(UItemObject* ItemObject) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UPlayerEquipmentComponent> PlayerEquipComp;
	UPROPERTY(BlueprintReadWrite)
	bool bInvalidSlot;
	UPROPERTY(BlueprintReadWrite)
	bool bOccupiedSlot;

	virtual bool HasItem(UItemObject* ItemObject) const override;

private:
	void ProcessActiveItem(UItemObject* ItemObject);

	virtual void OnRep_Items() override;
};
