// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBItem.h"
#include "DBConsumable.generated.h"

class UDBEffect;

/**
 *
 */
UCLASS()
class DARKBORNE_API ADBConsumable : public ADBItem
{
	GENERATED_BODY()

public:
	virtual bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName) override;

protected:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<UDBEffect> EffectClass;

private:
	UPROPERTY()
	TObjectPtr<ADBCharacter> OwningCharacter;
	FScriptDelegate Delegate;
};
