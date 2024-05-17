// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/StaticMeshComponent.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "../Framework/ActorComponents/DBInteractionComponent.h"
#include "../Inventory/DBEquipmentComponent.h"

ADBItem::ADBItem()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SMComp"));
	SMComp->SetupAttachment(RootComponent);

	SMComp->SetCollisionProfileName(FName("Item"));

	bCanInteract = false;
}

void ADBItem::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() && !GetOwner())
	{
		SMComp->SetSimulatePhysics(true);
		bCanInteract = true;
	}
}

void ADBItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBItem, bCanInteract);
	DOREPLIFETIME(ADBItem, ItemObj);
}

void ADBItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADBItem::Initialize(UItemObject* ItemObject)
{
	if (ensureAlways(ItemObject))
		ItemObj = ItemObject;
}

UItemObject* ADBItem::GetItemObject() const
{
	return ItemObj;
}

void ADBItem::BeginInteract(UDBInteractionComponent* InteractionComp)
{
	InteractionComp->ExecuteInteraction();
}

void ADBItem::ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character)
{
	ADBCharacter* DBCharacter = Cast<ADBCharacter>(Character);
	if (!DBCharacter)
		return;

	auto Inventory = DBCharacter->GetComponentByClass<UPlayerEquipmentComponent>();
	auto Equipment = DBCharacter->GetComponentByClass<UDBEquipmentComponent>();

	//Replace this with server and client RPCs that call these in server
	if (Equipment->TryAddItem(ItemObj, DBCharacter))
	{

	}
	else if (Inventory->TryAddItem(ItemObj, DBCharacter))
	{

	}
	else
	{
		InteractionComp->DeclareFailedInteraction();
	}
}

void ADBItem::InterruptInteract()
{

}

void ADBItem::BeginTrace()
{
}

void ADBItem::EndTrace()
{
}

FDisplayInfo ADBItem::GetDisplayInfo() const
{
	if (ItemObj) {
		FString Name = ItemObj->GetDisplayName().ToString();
		FString Action = TEXT("Pickup");
		return FDisplayInfo(Action, Name);
	}
	return FDisplayInfo(TEXT("ERROR"), TEXT("Missing ItemObj"));
}

bool ADBItem::CanInteract() const
{
	return bCanInteract;
}

void ADBItem::SetCanInteract(bool bAllowInteract)
{
	bCanInteract = bAllowInteract;
}

bool ADBItem::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	if (!PlayerCharacter) return false;
	if (!PlayerCharacter->GetMesh()) return false;
	if (!PlayerCharacter->GetMesh()->GetAnimInstance()) return false;

	auto AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && ensureAlways(AnimMontage)) {
		float result = AnimInstance->Montage_Play(AnimMontage);

		if (result == 0.f)
			return false;

		if (!SectionName.IsNone())
			AnimInstance->Montage_JumpToSection(SectionName);

		return true;
	}
	else
		return false;
}

void ADBItem::Pickup(AActor* InteractingActor)
{
	auto DBCharacter = Cast<ADBCharacter>(InteractingActor);
	if (!DBCharacter)
		return;

	auto PlayerEquipComp = DBCharacter->GetComponentByClass<UPlayerEquipmentComponent>();
	if (PlayerEquipComp)
	{
		PlayerEquipComp->TryAddItem(ItemObj, DBCharacter);
	}
}

void ADBItem::OnRep_bCanInteract()
{
	if (bCanInteract)
	{
		SMComp->SetSimulatePhysics(true);
	}
}


