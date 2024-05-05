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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TryAddItem(UItemObject* ItemObject);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItem(UItemObject* ItemObject);
	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject) override;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemObject*> GetItems() const;

	UFUNCTION(BlueprintCallable)
	const UItemObject* GetItemBySlotType(ESlotType SlotType) const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UPlayerEquipmentComponent> PlayerEquipComp;

	UPROPERTY(BlueprintReadWrite)
	bool bInvalidSlot;
};
