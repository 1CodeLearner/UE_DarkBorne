// Fill out your copyright notice in the Description page of Project Settings.


#include "DBDropItemManager.h"
#include "Interfaces/ItemInterface.h"
#include "../Items/DBItem.h"
#include "../Framework/BFL/ItemLibrary.h"
#include "../Test/J_TestCharacter.h"
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
TArray<FItem> ADBDropItemManager::GenerateItems(FName MonsterName)
{
	TArray<FItem> ItemsToGenerate;
	ItemsToGenerate.Empty();

	if (ensureAlways(DT_DropRate && !MonsterName.IsNone()))
	{
		FDropRate* dropRate = DT_DropRate->FindRow<FDropRate>(MonsterName, FString::Printf(TEXT("Context")));

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
				//FItem item = *ItemTable->FindRow<FItem>(RowNames[rand], FString::Printf(TEXT("Context")));

				//AssignRarity(item);
				//AssignEnchantment(item);
				//AssignSlotHolder(item);
				FItem tempItem = CreateItem(ItemTable, RowNames[rand]);
				if (tempItem.IsValid())
					ItemsToGenerate.Add(tempItem);
				else
					UE_LOG(LogTemp, Warning, TEXT("Invalid Item generated. Item will be ignored"));
			}
			else return TArray<FItem>();
		}
	}
	else return TArray<FItem>();
	return ItemsToGenerate;
}

FItem ADBDropItemManager::GenerateItemByName(FName ItemName, EItemType Type)
{
	UDataTable* DT = *ItemTableMap.Find(Type);

	if(DT && DT->FindRow<FItem>(ItemName,FString::Printf(TEXT("Context"))))
		return CreateItem(DT, ItemName);
	else 
		return FItem();
}

ADBItem* ADBDropItemManager::SpawnItem(AActor* Instigated, FItem _ItemToSpawn)
{
	ADBItem* Item = GetWorld()->SpawnActorDeferred<ADBItem>
		(
			_ItemToSpawn.SlotHolder.ItemClass,
			Instigated->GetTransform(),
			Instigated
		);

	if (Item)
	{
		return Item;
	}

	return nullptr;
}

void ADBDropItemManager::AdjustFinalStat(AActor* Instigated, const FItem& item, bool bIsAdd)
{
	AJ_TestCharacter* Character = Cast<AJ_TestCharacter>(Instigated);
	if (Character)
	{
		for (int i = 0; i < Character->FinalStat.Attributes.Num(); ++i) {
			UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *UEnum::GetValueAsString(Character->FinalStat.Attributes[i].AttributeType), Character->FinalStat.Attributes[i].Range.max);
		}
		for (int i = 0; i < Character->FinalStat.PhysDamages.Num(); ++i) {
			UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *UEnum::GetValueAsString(Character->FinalStat.PhysDamages[i].PhysicalDamageType), Character->FinalStat.PhysDamages[i].Range.max);
		}

		if (bIsAdd)
		{
			for (int i = 0; i < item.Enchantments.Attributes.Num(); ++i) {
				int32 index = (int32)item.Enchantments.Attributes[i].AttributeType;
				Character->FinalStat.Attributes[index] += item.Enchantments.Attributes[i];
			}
			for (int i = 0; i < item.Enchantments.PhysicalDamages.Num(); ++i) {
				int32 index = (int32)item.Enchantments.PhysicalDamages[i].PhysicalDamageType;
				Character->FinalStat.PhysDamages[index] += item.Enchantments.PhysicalDamages[i];
			}
		}
		else
		{
			for (int i = 0; i < item.Enchantments.Attributes.Num(); ++i) {
				int32 index = (int32)item.Enchantments.Attributes[i].AttributeType;
				Character->FinalStat.Attributes[index] -= item.Enchantments.Attributes[i];
			}
			for (int i = 0; i < item.Enchantments.PhysicalDamages.Num(); ++i) {
				int32 index = (int32)item.Enchantments.PhysicalDamages[i].PhysicalDamageType;
				Character->FinalStat.PhysDamages[index] -= item.Enchantments.PhysicalDamages[i];
			}
		}

		for (int i = 0; i < Character->FinalStat.Attributes.Num(); ++i) {
			UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *UEnum::GetValueAsString(Character->FinalStat.Attributes[i].AttributeType), Character->FinalStat.Attributes[i].Range.max);
		}
		for (int i = 0; i < Character->FinalStat.PhysDamages.Num(); ++i) {
			UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *UEnum::GetValueAsString(Character->FinalStat.PhysDamages[i].PhysicalDamageType), Character->FinalStat.PhysDamages[i].Range.max);
		}

	}

}


FItem ADBDropItemManager::CreateItem(UDataTable* Table, FName RowName)
{
	FItem* item = Table->FindRow<FItem>(RowName, FString::Printf(TEXT("Context")));
	if (!item)
		return FItem();

	FItem tempItem = *item;

	AssignRarity(tempItem);
	AssignEnchantment(tempItem);
	AssignSlotHolder(tempItem);

	return tempItem;
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

void ADBDropItemManager::AssignSlotHolder(FItem& Item)
{
	Item.Initialize();
}

void ADBDropItemManager::AssignRarity(FItem& Item)
{
	if (!ensureAlways(Item.IsValid() == false)) return;
	int max = Item.GetRarities().Num() - 1;
	int rand = FMath::RandRange(0, max);

	FRarity Rarity = Item.GetRarities()[rand];

	GenerateStatFromRange(Rarity.Range);

	Item.Rarities.Empty();
	Item.Rarities.Add(Rarity);
}

void ADBDropItemManager::AssignEnchantment(FItem& Item)
{
	if (!ensureAlways(!Item.IsValid())) return;

	if (!ensureAlwaysMsgf(Item.Rarities.Num() > 0, TEXT("Ensure AssignEffect() is called before AssignEnhancement()")))
		return;

	if (!ensureAlways(!Enchantments.IsEmpty()))
		return;

	if ((int)Item.GetSlotType() >= (int)ESlotType::_ENCHANTMENTMARK_)
		return;

	ERarityType rarityType = Item.GetRarities()[0].RarityType;
	int EnchantmentNum = int(rarityType);

	std::vector<bool> attributeCheck;
	TArray<FAttribute> AttributesGenerated;

	TArray<FPhysicalDamage> PhysicalDamageGenerated;
	std::vector<bool> PhysicalDamageCheck;

	int counter = 0;

	//No enchantments for common. 1 enchantment for rare. 2 enchantments for epic;
	while (counter < EnchantmentNum - 1)
	{
		int DataTableIndex = FMath::RandRange(0, Enchantments.Num() - 1);
		FName SlotTypeName = FName(UEnum::GetValueAsString<ESlotType>(Item.GetSlotType()));

		UDataTable* Enchantment = Enchantments[DataTableIndex];

		if (Enchantment) {
			switch ((EDarkBornStatType)DataTableIndex)
			{
			case EDarkBornStatType::ATTRIBUTE: {
				FAttributeHolder* Holder = Enchantment->FindRow<FAttributeHolder>(SlotTypeName, FString::Printf(TEXT("Enchantment")));

				if (!Holder || Holder->Attributes.IsEmpty())
					continue;

				ProcessEnchantment<FAttribute>(AttributesGenerated, Holder->Attributes, attributeCheck, counter);
				break;
			}
			case EDarkBornStatType::PHYSICALDAMAGE: {
				FPhysicalDamageHolder* Holder = Enchantment->FindRow<FPhysicalDamageHolder>(SlotTypeName, FString::Printf(TEXT("Enchantment")));

				if (!Holder || Holder->PhysicalDamages.IsEmpty())
					continue;
				ProcessEnchantment<FPhysicalDamage>(PhysicalDamageGenerated, Holder->PhysicalDamages, PhysicalDamageCheck, counter);
			}
			}
		}
	}

	FDarkBorneStats holder;

	holder.Attributes = AttributesGenerated;

	holder.PhysicalDamages = PhysicalDamageGenerated;

	Item.Enchantments = holder;
}

