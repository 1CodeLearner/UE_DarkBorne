// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAttackComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "../DBRogueCharacter.h"
#include "../../DBWeapon/DBRogueWeaponComponent.h"
#include "../../Items/Weapons/DBWeapon.h"


// Sets default values for this component's properties
UDBRogueAttackComponent::UDBRogueAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDBRogueAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDBRogueAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDBRogueAttackComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_DB_Attack, ETriggerEvent::Triggered, this, &UDBRogueAttackComponent::RogueAttack);

}

void UDBRogueAttackComponent::RogueAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Rogue Attack"));
	
	ADBRogueCharacter* RoguePlayer = Cast<ADBRogueCharacter>(GetOwner());

	// 단검을 들고 있으면 
	if (RoguePlayer->RogueWeaponComp->Dagger != nullptr)
	{
		// 단검 아이템에 있는 애님몽타주 실행
		RoguePlayer->RogueWeaponComp->Dagger->PlayMontage(RoguePlayer, FName("Attack4"));

		
	}
	
	
}

