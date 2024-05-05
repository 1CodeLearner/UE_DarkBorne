// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseInventoryComponent.generated.h"

class UItemObject;
class UActorChannel;
class FOutBunch;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDel);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBaseInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChangedDel OnInventoryChanged;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemObject* ItemObject);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* ItemObject);

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnItem(AActor* Initiator, UItemObject* ItemObject, bool bSetOwner = false, float forwardOffset = 200.f);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_RemoveItem(UItemObject* ItemObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rows = -1;
	
	UPROPERTY(ReplicatedUsing = OnRep_Items, VisibleAnywhere)
	TArray<UItemObject*> Items;
	
	UFUNCTION()
	void OnRep_Items(TArray<UItemObject*> OldItemArray);

	bool bIsDirty;

	FTransform GetNewTransform(AActor* Instigator, float offset);
};
