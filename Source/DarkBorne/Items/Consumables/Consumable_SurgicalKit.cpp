// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable_SurgicalKit.h"
#include "../../DBCharacters/DBCharacter.h"

bool AConsumable_SurgicalKit::PlayMontage(ACharacter* PlayerCharacter, FName SectionName)
{
	if (Super::PlayMontage(PlayerCharacter, SectionName))
	{
		if (OwningCharacter->HasAuthority())
		{
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &AConsumable_SurgicalKit::Testing, 10.f, false);
		}
		return true;
	}
	return false;
}

//void AConsumable_SurgicalKit::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	if (!bInterrupted)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Not Interrupted"));
//
//		auto EffectComp = OwningCharacter->GetComponentByClass<UDBEffectComponent>();
//		if (ensureAlways(EffectComp))
//		{
//			EffectComp->AddEffect(OwningCharacter, this);
//		}
//
//		auto EquipmentComp = OwningCharacter->GetComponentByClass<UDBEquipmentComponent>();
//		if (EquipmentComp)
//		{
//			EquipmentComp->RemoveItem(GetItemObject(), EquipmentComp);
//		}
//	}
//	else{
//		UE_LOG(LogTemp, Warning, TEXT("Interrupted"));
//	}
//
//	OwningCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.Remove(Delegate);
//	GetItemObject()->TryDestroyItemActor();
//}

void AConsumable_SurgicalKit::Testing()
{
	OwningCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection("StopMontage", AnimMontage);
}
