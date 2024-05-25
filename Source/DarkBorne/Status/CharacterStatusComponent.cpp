// Fill out your copyright notice in the Description page of Project Settings.


#include "../Status/CharacterStatusComponent.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyFSMBase.h"
#include "Net/UnrealNetwork.h"
#include "../Inventory/ItemObject.h"
#include "../Framework/BFL/DarkBorneLibrary.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
	bInitialized = false;
}

// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	MyActor = Cast<ACharacter>(GetOwner());

	// ...
	CurrHP = MaxHP;
}


void UCharacterStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterStatusComponent, MyActor);
	DOREPLIFETIME(UCharacterStatusComponent, MaxHP);
	DOREPLIFETIME(UCharacterStatusComponent, CurrHP);
}

// Called every frame
void UCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}

void UCharacterStatusComponent::DamageProcess(float damage, AActor* From)
{
	if (CurrHP <= 0) return;
	UE_LOG(LogTemp, Warning, TEXT("DamageProcess %f"), damage);
	if (!MyActor->HasAuthority() || MyActor == nullptr)
	{
		return;
	}

	if (AEnemyBase* enemy = Cast<AEnemyBase>(MyActor))
	{

		UE_LOG(LogTemp, Warning, TEXT("EnemyHit"));
		enemy->DamageProcess(damage, From);


	}
	else if (ADBCharacter* player = Cast<ADBCharacter>(MyActor))
	{
		float finalDamage = damage;
		//float BlockAmount = GetAddedStat().DamageBlockAmt;

		if (From) // if player was attacked by another player
		{
			//if (BlockAmount < finalDamage)
			//	finalDamage -= BlockAmount;
			//else
			//	finalDamage = 0.f;

			finalDamage = UDarkBorneLibrary::CalculateDamage(From, player);
		}
		UE_LOG(LogTemp, Warning, TEXT("FinalDamage : %f"), finalDamage);

		CurrHP -= finalDamage;
		if (CurrHP < 0)
		{
			CurrHP = 0;
			OnPlayerDead.ExecuteIfBound();
		}
		else if (CurrHP > MaxHP) {
			CurrHP = MaxHP;
		}
	}
}

bool UCharacterStatusComponent::IsAlive() const
{
	return CurrHP > 0.f;
}

void UCharacterStatusComponent::OnRep_CurrHP()
{
	if (GetOwner())
	{
		if (ADBCharacter* player = Cast<ADBCharacter>(GetOwner()))
		{
			player->OnRep_CurrHP();
		}
	}
}

void UCharacterStatusComponent::Initialize()
{
	// 서버라면
	if (!bInitialized && GetOwner()->HasAuthority())
	{
		if (ensureAlways(DT_CharacterStats))
		{
			FCharacterBaseStat* CharacterBaseStat = DT_CharacterStats->FindRow<FCharacterBaseStat>
				(
					RowName,
					FString::Printf(TEXT("Getting CharacterBaseStat"))
				);

			if (ensureAlways(CharacterBaseStat != nullptr) &&
				ensureAlways(CharacterBaseStat->Attributes.Num() == BaseStat.Attributes.Num()))
			{
				for (int32 i = 0; i < CharacterBaseStat->Attributes.Num(); ++i)
					BaseStat.Attributes[i] += CharacterBaseStat->Attributes[i];
			}
		}

		bInitialized = true;
	}
}

const FAddedStat& UCharacterStatusComponent::GetAddedStat() const
{
	return AddedStat;
}

const FBaseStat& UCharacterStatusComponent::GetBaseStat() const
{
	return BaseStat;
}

FFinalStat UCharacterStatusComponent::GetFinalStat() const
{
	float WeaponDmg = AddedStat.WeaponDamage;
	
	TArray<FAttribute> AddedAttributes;
	for(int i = 0; i < AddedStat.Attributes.Num(); ++i)
	{
		FAttribute Added = AddedStat.Attributes[i] + BaseStat.Attributes[i];
		AddedAttributes.Add(Added);
	}
	
	return FFinalStat(WeaponDmg, AddedAttributes, AddedStat.PhysDamages, AddedStat.DamageBlockAmt);
}

UCharacterStatusComponent* UCharacterStatusComponent::Get(ADBCharacter* Character)
{
	if (Character)
	{
		auto StatusComp = Character->GetComponentByClass<UCharacterStatusComponent>();
		if (StatusComp)
			return StatusComp;
	}

	return nullptr;
}

void UCharacterStatusComponent::AdjustAddedStats(AActor* Instigated, const UItemObject* ItemObject, bool bIsAdd)
{
	ADBCharacter* Character = Cast<ADBCharacter>(Instigated);
	if (Character)
	{
		UCharacterStatusComponent* StatusComp = UCharacterStatusComponent::Get(Character);

		if (StatusComp)
		{
			StatusComp->PrintStats();

			if (bIsAdd)
				StatusComp->AddStats(ItemObject);
			else
				StatusComp->RemoveStats(ItemObject);

			StatusComp->PrintStats();
		}
	}
}

void UCharacterStatusComponent::AddStats(const UItemObject* ItemObject)
{
	const FItem& Item = ItemObject->GetItem();

	if (ItemObject->GetSlotType() == ESlotType::WEAPON)
	{
		AddedStat.WeaponDamage += ItemObject->GetRarityValue();
	}

	for (int i = 0; i < Item.Enchantments.Attributes.Num(); ++i) {
		int32 index = (int32)Item.Enchantments.Attributes[i].AttributeType;
		AddedStat.Attributes[index] += Item.Enchantments.Attributes[i];
	}
	for (int i = 0; i < Item.Enchantments.PhysicalDamages.Num(); ++i) {
		int32 index = (int32)Item.Enchantments.PhysicalDamages[i].PhysicalDamageType;
		AddedStat.PhysDamages[index] += Item.Enchantments.PhysicalDamages[i];
	}
}

void UCharacterStatusComponent::RemoveStats(const UItemObject* ItemObject)
{
	const FItem& Item = ItemObject->GetItem();

	if (ItemObject->GetSlotType() == ESlotType::WEAPON)
	{
		AddedStat.WeaponDamage -= ItemObject->GetRarityValue();
	}

	for (int i = 0; i < Item.Enchantments.Attributes.Num(); ++i) {
		int32 index = (int32)Item.Enchantments.Attributes[i].AttributeType;
		AddedStat.Attributes[index] -= Item.Enchantments.Attributes[i];
	}

	for (int i = 0; i < Item.Enchantments.PhysicalDamages.Num(); ++i) {
		int32 index = (int32)Item.Enchantments.PhysicalDamages[i].PhysicalDamageType;
		AddedStat.PhysDamages[index] -= Item.Enchantments.PhysicalDamages[i];
	}
}

void UCharacterStatusComponent::AddBlockAmount(float Amount)
{
	//FinalStat.DamageBlocks.Add(Amount);
	AddedStat.DamageBlockAmt += Amount;
	UE_LOG(LogTemp, Warning, TEXT("DamageBlockAmt amount: %f"), AddedStat.DamageBlockAmt);
}

void UCharacterStatusComponent::RemoveBlockAmount(float Amount)
{
	AddedStat.DamageBlockAmt -= Amount;
	UE_LOG(LogTemp, Warning, TEXT("DamageBlockAmt Num: %d"), AddedStat.DamageBlockAmt);
}

void UCharacterStatusComponent::PrintStats()
{
	for (int i = 0; i < GetAddedStat().Attributes.Num(); ++i) {
		UE_LOG(LogTemp, Warning, TEXT("%s: %f"),
			*UEnum::GetValueAsString(GetAddedStat().Attributes[i].AttributeType),
			GetAddedStat().Attributes[i].Range.max);
	}
	for (int i = 0; i < GetAddedStat().PhysDamages.Num(); ++i) {
		UE_LOG(LogTemp, Warning, TEXT("%s: %f"),
			*UEnum::GetValueAsString(GetAddedStat().PhysDamages[i].PhysicalDamageType),
			GetAddedStat().PhysDamages[i].Range.max);
	}
}
