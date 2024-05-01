// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBInteractionComponent.generated.h"

class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UDBInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBInteractionComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractDistance;
};
