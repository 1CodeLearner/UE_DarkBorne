// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBConsumable.h"
#include "../../Framework/Interfaces/ItemInterface.h"
#include "Consumable_SurgicalKit.generated.h"

/**
 *
 */

UCLASS()
class DARKBORNE_API AConsumable_SurgicalKit : public ADBConsumable, public IItemInterface
{
	GENERATED_BODY()
public:
	virtual bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName) override;

protected:
	//virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void Testing();
};
