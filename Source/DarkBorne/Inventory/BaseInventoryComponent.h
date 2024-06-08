// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"

class UItemObject;
class UActorChannel;
class FOutBunch;
class ADBCharacter;

USTRUCT(Blueprintable)
struct FInventoryInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bHasRightClicked;
	UPROPERTY(BlueprintReadWrite)
	bool bHasShiftClicked;
};

USTRUCT()
struct FInventoryItems
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* InteractingActor;
	
	UPROPERTY(VisibleInstanceOnly)
	TArray<UItemObject*> Items;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDel);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseInventoryComponent();
	
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedDel OnInventoryChanged;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput);
	UFUNCTION(Server, Reliable)
	virtual void Server_TaxiForProcessPressInput(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput);
	UFUNCTION(Server, Reliable)
	virtual void Server_ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput);


	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemObject* ItemObject, AActor* InitiatedActor);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor);

	UFUNCTION(BlueprintCallable)
	virtual bool HasItem(UItemObject* ItemObject) const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnItem(AActor* Initiator, UItemObject* ItemObject, bool bSetOwner = false, float forwardOffset = 200.f);

protected:
	UFUNCTION(Server, Reliable)
	virtual void Server_RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor);

	UPROPERTY(ReplicatedUsing = OnRep_Items, VisibleAnywhere)
	FInventoryItems InventoryItems;

	UFUNCTION()
	virtual void OnRep_Items(FInventoryItems Old);

	FTransform GetNewTransform(AActor* Instigator, float offset);

private:
	bool bIsDirty;
};
