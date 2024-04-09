// Fill out your copyright notice in the Description page of Project Settings.


#include "DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"

ADBDropItemManager::ADBDropItemManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADBDropItemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FItem> ADBDropItemManager::GenerateItems(FName RowName)
{
	TArray<FItem> ItemsToGenerate;
	ItemsToGenerate.Empty();

	if (ensureAlways(DT_DropRate && !RowName.IsNone()))
	{
		FDropRate* dropRate = DT_DropRate->FindRow<FDropRate>(RowName, FString::Printf(TEXT("Context")));

		if (!ensureAlwaysMsgf(dropRate, TEXT("Could not find RowName")))
			return TArray<FItem>();

		if(!FindCumulativeProbability(dropRate))
			return TArray<FItem>();


		const int amount = dropRate->Amount;

		for (int i = 0; i < amount; ++i)
		{
			FDroppedItem itemDropped;
			const TArray<FDroppedItem>& items = dropRate->Items;
			float rate = FMath::RandRange(0.f, 1.f);

			for (int j = 0; j < CumulativeProbability.Num(); ++j)
			{
				if (rate <= CumulativeProbability[j])
				{
					 itemDropped = items[j];
					 break;
				}
			}

			if (ensure(ItemTableMap.Contains(itemDropped.ItemType)))
			{
				UDataTable* ItemTable = *ItemTableMap.Find(itemDropped.ItemType);
				TArray<FName> RowNames = ItemTable->GetRowNames();

				int rand = FMath::RandRange(0, RowNames.Num() - 1);
				FItem item = *ItemTable->FindRow<FItem>(RowNames[rand], FString::Printf(TEXT("Context")));

				ItemsToGenerate.Add(item);
			}
		}
	}
	else
	{
		return TArray<FItem>();
	}

	return ItemsToGenerate;
}

bool ADBDropItemManager::FindCumulativeProbability(const FDropRate* DropRate)
{
	CumulativeProbability.Empty();
	float sum = 0.f;

	for (auto item : DropRate->Items) {
		sum += item.Probability;
		CumulativeProbability.Add(sum);
	}

	if (!ensureAlwaysMsgf(sum == 1.f, TEXT("Total probability sum is not 100 percent"))) {
		CumulativeProbability.Empty();
		return false;
	}

	return true;
}
