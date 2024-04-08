// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DBRogueWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UDBRogueWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBRogueWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
