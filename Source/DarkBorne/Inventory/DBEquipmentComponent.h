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
	
protected:
	virtual void BeginPlay() override;

public:
	//아이템 추가 처리
	virtual bool TryAddItem(UItemObject* ItemObject, AActor* InitiatedActor) override;
	UFUNCTION(BlueprintCallable)
	void AddItem(UItemObject* ItemObject, AActor* InitiatedActor);
	UFUNCTION(Server, Reliable)
	void Server_TaxiForAddItem(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor);
	UFUNCTION(Server, Reliable)
	void Server_AddItem(UItemObject* ItemObject, AActor* InitiatedActor);

	//아이템 제거 처리
	virtual void RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor) override;
	UFUNCTION(Server, Reliable)
	void Server_TaxiForRemoveItem(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor);
	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject, AActor* InitiatedActor) override;

	//키보드 + 마우스 입력 처리
	virtual void ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;
	virtual void Server_TaxiForProcessPressInput_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;
	virtual void Server_ProcessPressInput_Implementation(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemObject*> GetSlots() const;

	UFUNCTION(BlueprintCallable)
	UItemObject* GetSlotItem(ESlotType SlotType) const;

	UFUNCTION(BlueprintCallable)
	bool IsSlotVacant(UItemObject* ItemObject) const;

protected:
	virtual bool HasItem(UItemObject* ItemObject) const override;

	UPROPERTY(BlueprintReadWrite)
	bool bInvalidSlot;
	UPROPERTY(BlueprintReadWrite)
	bool bOccupiedSlot;

private:
	virtual void OnRep_Items(FInventoryItems Old) override;
	bool bHasGameStarted = false;
};
