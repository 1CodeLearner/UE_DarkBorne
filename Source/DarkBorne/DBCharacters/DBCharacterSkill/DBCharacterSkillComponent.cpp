// Fill out your copyright notice in the Description page of Project Settings.


#include "DBCharacterSkillComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

// Sets default values for this component's properties
UDBCharacterSkillComponent::UDBCharacterSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UDBCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}





// Called every frame
void UDBCharacterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDBCharacterSkillComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent)
{
	enhancedInputComponent->BindAction(ia_Q_Skill, ETriggerEvent::Triggered, this, &UDBCharacterSkillComponent::QSkillSlot);
	enhancedInputComponent->BindAction(ia_E_Skill, ETriggerEvent::Triggered, this, &UDBCharacterSkillComponent::ESkillSlot);
	
}

void UDBCharacterSkillComponent::QSkillSlot(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("QSkill"));
}

void UDBCharacterSkillComponent::ESkillSlot(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("ESkill"));

}

