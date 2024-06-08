// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInventoryComponent.h"
#include "PlayerEquipmentComponent.generated.h"


class UItemObject;
class UActorChannel;
class FOutBunch;

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
	FLine() = default;
	FLine(FVector2D _Start, FVector2D _End)
	{
		Start = _Start;
		End = _End;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D Start = FVector2D::Zero();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FVector2D End = FVector2D::Zero();
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DARKBORNE_API UPlayerEquipmentComponent : public UBaseInventoryComponent
{
	GENERATED_BODY()

	friend class ULootInventoryComponent;

public:	
	UPlayerEquipmentComponent();
	
	UFUNCTION(BlueprintCallable)
	static float GetTileSize(UBaseInventoryComponent* BaseInventoryComp);

protected:
	virtual void BeginPlay() override;

public:
	bool HasRoomFor(UItemObject* ItemObject) const;
	virtual bool HasItem(UItemObject* ItemObject) const override;

	virtual bool TryAddItem(UItemObject* ItemObject, AActor* InitiatedActor) override;
	UFUNCTION(BlueprintCallable)
	void AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex, AActor* InitiatedActor);
	UFUNCTION(Server, Reliable)
	void Server_AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex, AActor* InitiatedActor);
	UFUNCTION(Server, Reliable)
	void Server_TaxiForAddItemAt(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, int32 TopLeftIndex, AActor* InitiatedActor);

	virtual void RemoveItem(UItemObject* ItemObject, AActor* InitiatedActor) override;
	UFUNCTION(Server, Reliable)
	void Server_TaxiForRemoveItem(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor);
	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject, AActor* InitiatedActor) override;

	virtual void ProcessPressInput(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;
	virtual void Server_ProcessPressInput_Implementation(UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;
	virtual void Server_TaxiForProcessPressInput_Implementation(UBaseInventoryComponent* TaxiedInventoryComp, UItemObject* ItemObject, AActor* InitiatedActor, FInventoryInput InventoryInput) override;

	UFUNCTION(BlueprintCallable)
	int32 GetColumn() const;

	UFUNCTION(BlueprintCallable)
	int32 GetRow() const;

	UFUNCTION(BlueprintCallable)
	FVector2D GetSize() const;
		
	UFUNCTION(BlueprintCallable)
	TMap<UItemObject*, FTile> GetAllItems() const;

	UFUNCTION(BlueprintCallable)
	FTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile Tile) const;

	TTuple<bool, UItemObject*> GetItematIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool IsTileValid(FTile tile) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rows = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;

private:
	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex) const;

	virtual void OnRep_Items(FInventoryItems Old) override;
};
