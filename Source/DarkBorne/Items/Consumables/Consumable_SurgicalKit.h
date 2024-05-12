// Fill out your copyright notice in the Description page of Project Settings.
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
	AConsumable_SurgicalKit();
	virtual bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName) override;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_Owner() override;

protected:
	
	UFUNCTION()
	void OnRep_currTime();
	UPROPERTY(ReplicatedUsing = "OnRep_currTime")
	float currTime;

	UPROPERTY(Replicated)
	float totalTime;
				
	UFUNCTION()
	void OnRep_bTimeElapsed();
	UPROPERTY(ReplicatedUsing = "OnRep_bTimeElapsed")
	bool bTimeElapsed;

	bool bMontageStart;
};
