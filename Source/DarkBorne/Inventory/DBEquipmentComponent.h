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

	virtual bool TryAddItem(UItemObject* ItemObject) override;
	virtual void RemoveItem(UItemObject* ItemObject) override;
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItem(UItemObject* ItemObject);
	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject) override;

	UFUNCTION(BlueprintCallable)
	const TArray<UItemObject*> GetSlots() const;

	UFUNCTION(BlueprintCallable)
	const UItemObject* GetSlotItem(ESlotType SlotType) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UPlayerEquipmentComponent> PlayerEquipComp;
	UPROPERTY(BlueprintReadWrite)
	bool bInvalidSlot;
};
