// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ItemTypes/ItemType.h"
#include "DBEffectComponent.generated.h"


class UDBEffect;
class ADBCharacter;
class UMaterialInterface;
class ADBConsumable;

//DECLARE_DELEGATE_TwoParams(FEffectStartDelegate, UDBEffect* /*effect*/, UMaterialInterface* /*IconDisplay*/);

DECLARE_DELEGATE_OneParam(EffectUpdateDelegate, TArray<UDBEffect*> UpdatedEffects);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UDBEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBEffectComponent();

	void AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate);

	void RemoveEffect(UDBEffect* Effect);

	void RemoveAllEffects();

	UFUNCTION(BlueprintCallable)
	bool CanStartEffect(ADBConsumable* ItemToActivate);

	//FEffectStartDelegate OnEffectStart;
	EffectUpdateDelegate OnEffectUpdate;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;


	UFUNCTION(Server, Reliable)
	void Server_AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate);

private:
	UPROPERTY(ReplicatedUsing = "OnRep_Effects", VisibleAnywhere, Category = "Settings")
	TArray<UDBEffect*> Effects;
	UFUNCTION()
	void OnRep_Effects();
};
