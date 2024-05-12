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


	UFUNCTION()
	void StopMontage();

protected:
	//UPROPERTY(ReplicatedUsing = "OnRep_currTime")
	float currTime;
	//void OnRep_currTime();
	//UPROPERTY(Replicated)
	float totalTime;
};
