// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBDropItemManager.generated.h"

enum class EItemType : uint8;
struct FDropRate; 
struct FItem;
struct FEffect; 

UCLASS()
class DARKBORNE_API ADBDropItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADBDropItemManager();
		
	UFUNCTION(BlueprintCallable)
	TArray<FItem> GenerateItems(FName RowName);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<EItemType, UDataTable*> ItemTableMap;
	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataTable* DT_DropRate;

private:
	bool FindCumulativeProbability(const FDropRate* DropRate);
	void AssignEffect(FItem& Item);
	TArray<float> CumulativeProbability;
};
