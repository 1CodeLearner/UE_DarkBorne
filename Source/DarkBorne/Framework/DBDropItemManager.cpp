// Fill out your copyright notice in the Description page of Project Settings.


#include "DBDropItemManager.h"
#include "../ItemTypes/ItemType.h"
#include "../Items/PDA_ItemSlot.h"

ADBDropItemManager::ADBDropItemManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADBDropItemManager::BeginPlay()
{
	Super::BeginPlay();

}
/// <summary>
/// Creates array of FItem instances for inventory. 
/// Each FItem instance is assigned a unique stat named Effect.
///  
/// </summary>
/// <param name="RowName"> : Name of a monster from which to generate items. Name must match the row names of DT_DropRate data table.
/// </param>
/// <returns>
/// Returns instances of generated items. Returns empty if an error occured.
/// </returns>
TArray<FItem> ADBDropItemManager::GenerateItems(FName RowName)
{
	TArray<FItem> ItemsToGenerate;
	ItemsToGenerate.Empty();

	if (ensureAlways(DT_DropRate && !RowName.IsNone()))
	{
		FDropRate* dropRate = DT_DropRate->FindRow<FDropRate>(RowName, FString::Printf(TEXT("Context")));

		if (!ensureAlwaysMsgf(dropRate, TEXT("Could not find RowName")))
			return TArray<FItem>();

		if (!FindCumulativeProbability(dropRate))
			return TArray<FItem>();


		const int amount = dropRate->Amount;
		const TArray<FDroppedItem>& items = dropRate->Items;

		for (int i = 0; i < amount; ++i)
		{
			FDroppedItem itemDropped;
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

				AssignEffect(item);
				AssignEnchantment(item);
				ItemsToGenerate.Add(item);
			}
			else return TArray<FItem>();
		}
	}
	else return TArray<FItem>();
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

void ADBDropItemManager::AssignEffect(FItem& Item)
{
	int max = Item.ItemSlot->Effects.Num() - 1;
	int rand = FMath::RandRange(0, max);

	FEffect Effect = Item.ItemSlot->Effects[rand];

	GenerateStatFromRange(Effect.Range);

	Item.Effects.Empty();
	Item.Effects.Add(Effect);
}

void ADBDropItemManager::AssignEnchantment(FItem& Item)
{
	if (!ensureAlwaysMsgf(Item.Effects.Num() > 0, TEXT("Ensure AssignEffect() is called before AssignEnhancement()")))
		return;

	if (!ensureAlways(!Enchantments.IsEmpty()))
		return;

	if ((int)Item.ItemSlot->SlotType >= (int)ESlotType::_ENCHANTMENTMARK_)
		return;

	ERarity rarity = Item.Effects[0].Rarity;
	int EnchantmentNum = int(rarity);

	std::vector<bool> attributeCheck;
	TArray<FAttribute> AttributesGenerated;

	int counter = 0;

	while (counter < 2)
	{
		int DataTableIndex = FMath::RandRange(0, Enchantments.Num() - 1);
		FName SlotTypeName = FName(UEnum::GetValueAsString<ESlotType>(Item.ItemSlot->SlotType));

		UDataTable* Enchantment = Enchantments[DataTableIndex];

		if (Enchantment) {
			switch (EEnchantmentType(DataTableIndex))
			{
			case EEnchantmentType::ATTRIBUTE: {
				FAttributeHolder* Holder = Enchantment->FindRow<FAttributeHolder>(SlotTypeName, FString::Printf(TEXT("Enchantment")));

				if(!Holder || Holder->Attributes.IsEmpty()) 
					break;

				UE_LOG(LogTemp, Warning, TEXT("ATTRIBUTE FOUND BRO, %s"), Holder ? TEXT("YES") : TEXT("NO"));

				if (attributeCheck.size() == 0)
					attributeCheck.resize(Holder->Attributes.Num(), false);

				int randIndex = FMath::RandRange(0, Holder->Attributes.Num() - 1);
				if (attributeCheck[randIndex]) 
					break;

				FAttribute NewAttribute = Holder->Attributes[randIndex];

				GenerateStatFromRange(NewAttribute.Range);

				AttributesGenerated.Add(NewAttribute);
				attributeCheck[randIndex] = true;
				counter++;

				break;
			}
			}
		}
	}

	FEnchantmentsHolder holder;

	holder.Attributes = AttributesGenerated;

	Item.Enchantments = holder;
}