// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "../Items/PDA_ItemSlot.h"
#include "Materials/MaterialInterface.h"
#include "../Items/DBItem.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "../Items/DBItem.h"

void UItemObject::Initialize(FItem _Item, ADBItem* _ItemActor)
{
	ItemData = FItemData(_Item, _ItemActor);
}

ADBItem* UItemObject::SpawnItem(AActor* Initiator, bool bSetOwner, FTransform Trans, float forwardOffset)
{
	if(!ensureAlways(!GetItemActor())) return nullptr;
		
	auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);
	ItemSpawned->Initialize(this);
	UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
	
	SetItemActor(ItemSpawned);
	
	return ItemSpawned;	
}

void UItemObject::SetItemActor(ADBItem* _ItemActor)
{
	if (ensureAlways(_ItemActor))
		ItemData.ItemActor = _ItemActor;
}

bool UItemObject::HasItemActor() const
{
	return IsValid(ItemData.ItemActor);
}

ADBItem* UItemObject::GetItemActor() const
{
	return ItemData.ItemActor;
}

void UItemObject::TryDestroyItemActor()
{
	AActor* ItemActor = GetItemActor();

	if (IsValid(ItemActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Actor %s, Destroying.."), *ItemActor->GetName());
		ItemData.ItemActor = nullptr;
		ItemActor->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Has no Actor"));
	}
}

FText UItemObject::GetDisplayName() const
{
	return ItemData.Item.SlotHolder.DisplayName;
}

FName UItemObject::GetId() const
{
	return ItemData.Item.SlotHolder.Id;
}

float UItemObject::GetRarityValue() const
{
	return ItemData.Item.GetDefaultValue();
}

FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	//return _dimentions;
	FIntPoint Temp(ItemData.Item.SlotHolder.SlotDimension.X, ItemData.Item.SlotHolder.SlotDimension.Y);
	return Temp;
}

UMaterialInterface* UItemObject::GetIcon()
{
	return ItemData.Item.SlotHolder.DisplayMaterial;
}

TSubclassOf<AActor> UItemObject::GetItemClass()
{
	return ItemData.Item.SlotHolder.ItemClass;
}

ESlotType UItemObject::GetSlotType() const
{
	return ItemData.Item.SlotHolder.SlotType;
}

const FItem& UItemObject::GetItem() const
{
	return ItemData.Item;
}

//void UItemObject::Server_SpawnItem_Implementation(AActor* Initiator, float forwardOffset, bool bSetOwner)
//{
//	///UE_LOG(LogTemp, Warning, TEXT("Net? %s"),
//		GetWorld()->GetNetMode() == NM_Client ? TEXT("CLIENT") : TEXT("SERVER")
//	);
//	if (Initiator && GetWorld()) {
//		FVector SpawnLoc = Initiator->GetActorLocation();
//		SpawnLoc += Initiator->GetActorForwardVector() * forwardOffset;
//		FTransform Trans;
//		Trans.SetLocation(SpawnLoc);
//		Trans.SetRotation(FQuat::Identity);
//		Trans.SetScale3D(FVector::OneVector);
//
//		auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);
//
//		ItemSpawned->Initialize(this);
//
//		UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Spawn Item Failed in %s"), *GetNameSafe(this));
//		return;
//	}
//}

UWorld* UItemObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

TStatId UItemObject::GetStatId() const
{
	return TStatId();
}

void UItemObject::Tick(float DeltaTime)
{
	//if (GetWorld())
	//	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf
	//	(
	//		TEXT("[%s] %s"),
	//		(GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
	//		*ItemData.Item.SlotHolder.DisplayName.ToString()
	//	)
	//	);
}

void UItemObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, ItemData);
}
