// Fill out your copyright notice in the Description page of Project Settings.


#include "DBItem.h"
#include "Components/StaticMeshComponent.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include "GameFramework/Character.h"
#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "../Framework/ActorComponents/DBInteractionComponent.h"

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

void ADBItem::BeginInteract_Implementation(UDBInteractionComponent* InteractionComp)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact %s, %s"), *GetNameSafe(InteractionComp),
		GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server")
	);
	InteractionComp->InteractExecute();
}

void ADBItem::ExecuteInteract_Implementation(ACharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute"));
}

void ADBItem::InterruptInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interrupt"));
}

void ADBItem::BeginTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate material"));
}

void ADBItem::EndTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate material"));
}

UItemObject* ADBItem::GetItemObject_Implementation() const
{
	return ItemObj;
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
	auto PlayerEquipComp = InteractingActor->GetComponentByClass<UPlayerEquipmentComponent>();
	if (PlayerEquipComp)
	{
		PlayerEquipComp->TryAddItem(ItemObj);
	}
}

void ADBItem::OnRep_bCanInteract()
{
	if (bCanInteract)
	{
		SMComp->SetSimulatePhysics(true);
	}
}


