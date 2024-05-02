// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChangedDelegate);

class UItemObject;
class UPlayerEquipmentComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DARKBORNE_API UDBEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ULootInventoryComponent;
public:
	UDBEquipmentComponent();

	bool TryAddItem(UItemObject* ItemObject);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItem(UItemObject* ItemObject);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItem(UItemObject* ItemObject);
	UFUNCTION(BlueprintCallable)
	const TArray<UItemObject*> GetSlots() const;

	UFUNCTION(BlueprintCallable)
	const UItemObject* GetSlotItem(ESlotType SlotType) const;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEquipmentChangedDelegate OnEquipmentChanged;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(ReplicatedUsing = OnRep_What, VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<UItemObject*> Slots;

	UFUNCTION()
	void OnRep_What(TArray<UItemObject*> OldSlots);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<UPlayerEquipmentComponent> PlayerEquipComp;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDirty;
	UPROPERTY(BlueprintReadWrite)
	bool bInvalidSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings")
	int Columns;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	int Rows;


};
