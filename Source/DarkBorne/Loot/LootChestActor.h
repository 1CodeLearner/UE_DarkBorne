// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Framework/Interfaces/InteractionInterface.h"
#include "LootChestActor.generated.h"

class ULootInventoryComponent;
class USoundBase;

UCLASS()
class DARKBORNE_API ALootChestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ALootChestActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<ULootInventoryComponent> LootInventoryComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName ItemDropName;

	//Being Looted Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAudioComponent> AudioComp_Looting;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAudioComponent> AudioComp_Open;

protected:
	// Inherited via IInteractionInterface
	void BeginInteract(UDBInteractionComponent* InteractionComp) override;

	void ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character) override;

	void InterruptInteract() override;

	void BeginTrace() override;

	void EndTrace() override;

	bool CanInteract() const override;

	void SetCanInteract(bool bAllowInteract) override;

	FDisplayInfo GetDisplayInfo() const override;
};
