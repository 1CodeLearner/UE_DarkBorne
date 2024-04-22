// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Inventory/ItemObject.h"
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

public:
	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_WeaponSlot;

	UPROPERTY(Replicated)
	class ADBWeapon_CloseRange* Dagger;
	
	UPROPERTY()
	class ADBItem* RogueItems;

	UPROPERTY(EditAnywhere)
	TArray<UItemObject*> EquipSlotArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADBWeapon_CloseRange> DaggerFactory;

	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> RogueItemSMMat;

public:
	bool hasWeapon;
public:
	void AttachWeapon();
	UFUNCTION(Server, Reliable)
	void ServerRPC_AttachWeapon();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_AttachWeapon();

public:
	void PassItem(UItemObject* Item);
};
