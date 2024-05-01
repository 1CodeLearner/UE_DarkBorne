// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"
#include "../Items/PDA_ItemSlot.h"
#include "Materials/MaterialInterface.h"
#include "../Items/DBItem.h"
#include "Net/UnrealNetwork.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void UItemObject::Initialize(FItem item)
{
	this->Item = item;
}

FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	//return _dimentions;
	FIntPoint Temp(Item.SlotHolder.SlotDimension.X, Item.SlotHolder.SlotDimension.Y);
	return Temp;
}

UMaterialInterface* UItemObject::GetIcon()
{
	return Item.SlotHolder.DisplayMaterial;
}

TSubclassOf<AActor> UItemObject::GetItemClass()
{
	return Item.SlotHolder.ItemClass;
}

ESlotType UItemObject::GetSlotType() const
{
	return Item.SlotHolder.SlotType;
}

const FItem& UItemObject::GetItem() const
{
	return Item;
}

void UItemObject::Server_SpawnItem_Implementation(AActor* Initiator, float forwardOffset, bool bSetOwner)
{
	if (Initiator && GetWorld()) {
		FVector SpawnLoc = Initiator->GetActorLocation();
		SpawnLoc += Initiator->GetActorForwardVector() * forwardOffset;
		FTransform Trans;
		Trans.SetLocation(SpawnLoc);
		Trans.SetRotation(FQuat::Identity);
		Trans.SetScale3D(FVector::OneVector);

		auto ItemSpawned = GetWorld()->SpawnActorDeferred<ADBItem>(GetItemClass(), Trans, bSetOwner ? Initiator : nullptr);

		ItemSpawned->Initialize(this);

		UGameplayStatics::FinishSpawningActor(ItemSpawned, Trans);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Spawn Item Failed in %s"), *GetNameSafe(this));
		return;
	}
}

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
	if (GetWorld())
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf
		(
			TEXT("[%s] %s"),
			(GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server")),
			*Item.SlotHolder.DisplayName.ToString()
		)
		);
}

void UItemObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, Item);
}
