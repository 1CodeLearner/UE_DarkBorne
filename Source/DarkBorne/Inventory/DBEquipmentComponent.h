// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBEquipmentComponent.generated.h"

class UItemObject; 
class UPlayerEquipmentComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKBORNE_API UDBEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDBEquipmentComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddItem(UItemObject* ItemObject);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItem(UItemObject* ItemObject);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UPROPERTY(ReplicatedUsing = OnRep_What, VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<UItemObject*> Slots;

	UFUNCTION()
	void OnRep_What(TArray<UItemObject*> OldSlots);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	TObjectPtr<UPlayerEquipmentComponent> PlayerEquipComp;
};
