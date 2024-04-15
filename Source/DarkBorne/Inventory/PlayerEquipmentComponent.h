// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEquipmentComponent.generated.h"



DECLARE_DELEGATE(FOnInventoryChangedDel)


USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	int32 Y;
};

USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	FVector2D Start;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings")
	FVector2D End;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UPlayerEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(class UItemObject* ItemObject);

	TMap<class UItemObject*, FTile> GetAllItems();


	UFUNCTION(BlueprintCallable)
	FTile IndexToTile(int32 Index);

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FTile Tile);

	UFUNCTION(BlueprintCallable)
	void AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(class UItemObject* ItemObject);

	
	TTuple<bool,class UItemObject*> GetItematIndex(int32 Index);
	
	UFUNCTION(BlueprintCallable)
	bool IsTileValid(FTile tile);
	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns = -1;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Rows = -1;

private:
	TArray<class UItemObject*> itemArray;

	bool isDirty = false;
	
	
public:
	FOnInventoryChangedDel onInventoryChangedDel;
};
