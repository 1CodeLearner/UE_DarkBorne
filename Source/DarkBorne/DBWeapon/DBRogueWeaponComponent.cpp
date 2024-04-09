// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueWeaponComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../Items/Weapons/DBWeapon.h"

// Sets default values for this component's properties
UDBRogueWeaponComponent::UDBRogueWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDBRogueWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDBRogueWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDBRogueWeaponComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_WeaponSlot, ETriggerEvent::Triggered, this, &UDBRogueWeaponComponent::AttachWeapon);
		
}

void UDBRogueWeaponComponent::AttachWeapon()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Attach Weapon"));
	Dagger = GetWorld()->SpawnActor<ADBWeapon>(DaggerFactory, GetComponentLocation(), GetComponentRotation());
	if (Dagger)
	{
		Dagger->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	}

	
}

