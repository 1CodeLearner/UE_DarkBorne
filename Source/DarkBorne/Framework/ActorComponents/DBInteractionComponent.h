// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBInteractionComponent.generated.h"


DECLARE_DELEGATE_OneParam(FInteractActorUpdateDelegate, AActor* /*InteractingCharacter*/);

class ACharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UDBInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ADBCharacter;

public:
	UDBInteractionComponent();

	FInteractActorUpdateDelegate OnInteractActorUpdate;

	void InteractExecute();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float interactSpeed;

private:
	void OnInteract(bool bIsInput);

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<AActor> OverlappingActor;

	bool bInteracting;

	bool CanInteract(bool bDebugDraw);
	void UpdateOverlappingActor(bool bDebugDraw);
	void UpdateTimer();	
};
