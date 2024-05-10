// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ItemTypes/ItemType.h"
#include "DBEffectComponent.generated.h"


class UDBEffect;
class ADBCharacter;
class UMaterialInterface; 

DECLARE_DELEGATE_TwoParams(FInitStartDelegate, UDBEffect* /*effect*/, UMaterialInterface* /*IconDisplay*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UDBEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDBEffectComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddEffect(ADBCharacter* Instigated, TSubclassOf<UDBEffect> EffectClass, UItemObject* ItemObject);

	void RemoveEffect(UDBEffect* Effect);

	bool CanStartEffect(UDBEffect* Effect);
	
	FInitStartDelegate OnInitStart;

protected:
	virtual void BeginPlay() override;

private:	
	TArray<UDBEffect*> Effects;
		
};
