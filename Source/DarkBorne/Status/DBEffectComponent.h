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

DECLARE_DELEGATE_TwoParams(FEffectStartDelegate, UDBEffect* /*effect*/, UMaterialInterface* /*IconDisplay*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UDBEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDBEffectComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate);

	void RemoveEffect(UDBEffect* Effect);

	void RemoveAllEffects();

	UFUNCTION(BlueprintCallable)
	bool CanStartEffect(ADBConsumable* ItemToActivate);
	
	FEffectStartDelegate OnEffectStart;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server,Reliable)
	void Server_AddEffect(ADBCharacter* Instigated, ADBConsumable* ItemToActivate);

private:
	UPROPERTY(VisibleAnywhere, Category="Settings")
	TArray<UDBEffect*> Effects;		
};
