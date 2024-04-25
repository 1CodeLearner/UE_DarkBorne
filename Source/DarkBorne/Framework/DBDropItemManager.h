// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBDropItemManager.generated.h"

enum class EItemType : uint8;
struct FDropRate;
struct FEffect;

UCLASS()
class DARKBORNE_API ADBDropItemManager : public AActor
{
	GENERATED_BODY()

	friend class ATP_ThirdPersonGameMode;

public:
	ADBDropItemManager();


protected:
	UFUNCTION(BlueprintCallable)
	TArray<FItem> GenerateItems(FName RowName);

	UFUNCTION(BlueprintCallable)
	FItem GenerateItemByName(FName ItemName, EItemType Type);

	UFUNCTION(BlueprintCallable)
	ADBItem* SpawnItem(AActor* Instigated, FItem _ItemToSpawn);

	UFUNCTION(BlueprintCallable)
	static void AdjustFinalStat(AActor* Instigated, const FItem& item, bool bIsAdd);

	

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<EItemType, UDataTable*> ItemTableMap;
	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataTable* DT_DropRate;
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<UDataTable*> Enchantments;

private:
	FItem InitializeItem(UDataTable* Table, FName RowName);

	bool FindCumulativeProbability(const FDropRate* DropRate);
	void AssignSlotHolder(FItem& Item);
	void AssignRarity(FItem& Item);
	void AssignEnchantment(FItem& Item);

	template<typename U>
	void ProcessEnchantment(TArray<U>& Generated, const TArray<U>& Enchantment, std::vector<bool>& checkDuplicate, int& counter);

	template<class T>
	void GenerateStatFromRange(T& Range);
	TArray<float> CumulativeProbability;
};

template<class T>
inline void ADBDropItemManager::GenerateStatFromRange(T& Range)
{
	if (Range.min != Range.max)
	{
		int rand = FMath::RandRange(Range.min, Range.max + .5f);
		Range.min = rand;
		Range.max = rand;
	}
}



template<typename U>
void ADBDropItemManager::ProcessEnchantment(TArray<U>& Generated, const TArray<U>& Enchantment, std::vector<bool>& checkDuplicate, int& counter)
{
	if (checkDuplicate.size() == 0)
		checkDuplicate.resize(Enchantment.Num(), false);

	int randIndex = FMath::RandRange(0, Enchantment.Num() - 1);
	if (checkDuplicate[randIndex])
		return;

	U NewEnchantment = Enchantment[randIndex];

	GenerateStatFromRange(NewEnchantment.Range);

	Generated.Add(NewEnchantment);
	checkDuplicate[randIndex] = true;
	counter++;
}