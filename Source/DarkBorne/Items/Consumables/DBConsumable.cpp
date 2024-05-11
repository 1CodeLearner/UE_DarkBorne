// Fill out your copyright notice in the Description page of Project Settings.


#include "DBConsumable.h"
#include "../../DBCharacters/DBCharacter.h"
#include "Animation/AnimInstance.h"
#include "../../Status/DBEffectComponent.h"
#include "../../Inventory/DBEquipmentComponent.h"
#include "../../Inventory/ItemObject.h"

bool ADBConsumable::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	if (Super::PlayMontage(PlayerCharacter, SectionName))
	{
		OwningCharacter = Cast<ADBCharacter>(PlayerCharacter);
		//if (OwningCharacter && OwningCharacter->IsLocallyControlled())
		if (OwningCharacter->HasAuthority())
		{
			UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
			AnimInstance->OnMontageEnded.AddDynamic(this, &ADBConsumable::OnMontageEnded);

			Delegate.BindUFunction(this, "OnMontageEnded");
			AnimInstance->OnMontageEnded.AddUnique(Delegate);

			return true;
		}
	}

	return false;
}

TSubclassOf<UDBEffect> ADBConsumable::GetEffectClass() const
{
	return EffectClass;
}

void ADBConsumable::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		auto EffectComp = OwningCharacter->GetComponentByClass<UDBEffectComponent>();
		if (ensureAlways(EffectComp))
		{
			EffectComp->AddEffect(OwningCharacter, this);
		}

		auto EquipmentComp = OwningCharacter->GetComponentByClass<UDBEquipmentComponent>();
		if (EquipmentComp)
		{
			EquipmentComp->RemoveItem(GetItemObject(), EquipmentComp);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("OnMontageEnded in consumable invoked"));
	OwningCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.Remove(Delegate);

	GetItemObject()->TryDestroyItemActor();
}