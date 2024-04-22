// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEquipmentComponent.generated.h"


//for blueprint use
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedDel);


USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 X = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	int32 Y = 0;
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D Start = FVector2D::Zero();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D End = FVector2D::Zero();
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DARKBORNE_API UPlayerEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ULootInventoryComponent;

public:
	// Sets default values for this component's properties
	UPlayerEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sends Server RPC when item can be added
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(class UItemObject* ItemObject);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItem(class UItemObject* ItemObject);

	//Holds ItemObject until mouse button is released
	/*UPROPERTY(BlueprintReadOnly)
	UItemObject* ItemObjectHolder;*/

public:
	UFUNCTION(BlueprintCallable)
	TMap<class UItemObject*, FTile> GetAllItems() const;

	UFUNCTION(BlueprintCallable)
	FTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile Tile) const;

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex) const;

	TTuple<bool, class UItemObject*> GetItematIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool IsTileValid(FTile tile) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rows = -1;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_itemArray)
	TArray<class UItemObject*> itemArray;
	
	UFUNCTION()
	void OnRep_itemArray(TArray<UItemObject*> OldItemArray);

	UPROPERTY(EditAnywhere)
	bool isDirty;

protected:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInventoryChangedDel onInventoryChangedDel;
};
