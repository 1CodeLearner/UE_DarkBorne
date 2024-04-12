// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEquipmentComponent.generated.h"

struct FItem;


USTRUCT()
struct FTile
{
	GENERATED_BODY()
	int32 X;
	int32 Y;
};

USTRUCT()
struct FLine
{
	GENERATED_BODY()

	FVector2D Start;
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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	bool TryAddItem(class UItemObject* ItemObject);

	//UFUNCTION()
	//TMap<UItemObject, FTile> GetAllItems();


	UFUNCTION()
	FTile IndexToTile(int32 Index);

	UFUNCTION()
	int32 TileToIndex(FTile Tile);

	UFUNCTION()
	void AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION()
	bool IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex);

	UFUNCTION()
	bool RemoveItem(class UItemObject* ItemObject);

	
	TTuple<bool,class UItemObject*> GetItematIndex(int32 Index);
	
	UFUNCTION()
	bool IsTileValid(FTile tile);
	


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Rows;

private:
	TArray<class UItemObject*> itemArray;

	bool isDirty = false;
	
	
		
};
