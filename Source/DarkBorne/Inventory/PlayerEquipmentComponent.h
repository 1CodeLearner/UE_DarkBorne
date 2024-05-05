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

protected:
	virtual void BeginPlay() override;

public:
	virtual bool TryAddItem(UItemObject* ItemObject) override;

	virtual void RemoveItem(UItemObject* ItemObject) override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItemAt(UItemObject* ItemObject, int32 TopLeftIndex);

	virtual void Server_RemoveItem_Implementation(UItemObject* ItemObject) override;

	//Holds ItemObject until mouse button is released
	/*UPROPERTY(BlueprintReadOnly)
	UItemObject* ItemObjectHolder;*/

public:
	UFUNCTION(BlueprintCallable)
	TMap<UItemObject*, FTile> GetAllItems() const;

	UFUNCTION(BlueprintCallable)
	FTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile Tile) const;

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemObject* ItemObject, int32 TopLeftIndex) const;

	TTuple<bool, UItemObject*> GetItematIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool IsTileValid(FTile tile) const;
};
