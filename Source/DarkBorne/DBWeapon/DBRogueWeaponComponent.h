// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Inventory/ItemObject.h"
#include "DBRogueWeaponComponent.generated.h"

class UDBRogueAttackComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

public:
	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_WeaponSlot;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_ConsumableSlot;

	UPROPERTY(Replicated)
	class ADBWeapon_CloseRange* Dagger;

	UPROPERTY(Replicated)
	class ADBItem* RogueItems;
	//UFUNCTION()
	//void OnRep_RogueItems();

	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<UItemObject*> EquipSlotArray;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> RogueItemSMMat;

public:
	UPROPERTY(Replicated)
	bool hasWeapon = false;
public:
	void AttachWeapon();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRPC_AttachWeapon();

public:
	void PassItem(UItemObject* Item);
	void TryRemoveRogueItem(UItemObject* Item);



protected:
	//????
	void AttachConsumable();
	UFUNCTION(Server, Reliable)
	void Server_AttachConsumable();
	/*UFUNCTION(NetMulticast, Reliable)
	void Multicast_TestAttachItem(ADBItem* Test);*/
private:
	bool HandleAttach(int32 index);
	UPROPERTY()
	TObjectPtr<UDBRogueAttackComponent> AttackComp;
};
