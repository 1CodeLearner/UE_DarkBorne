// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBRogueAttackComponent.generated.h"


class ADBRogueCharacter;
class UDBRogueAnimInstance;
class UDBRogueSkillComponent;
class UDBRogueWeaponComponent;

USTRUCT()
struct FItemActionTime
{
	GENERATED_BODY()

	FItemActionTime()
	{
		CurrTime = 0.f;
		TotalTime = 0.f;
	}

	UPROPERTY()
	float CurrTime;

	UPROPERTY()
	float TotalTime;
};

//Invokes when player is using an item begin held
DECLARE_DELEGATE(FBeginItemActionDelegate);
DECLARE_DELEGATE(FEndItemActionDelegate);
DECLARE_DELEGATE_TwoParams(FItemActionUpdateDelegate, float /*CurrentTime*/, float /*MaxTime*/);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UDBRogueAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDBRogueAttackComponent();

	FBeginItemActionDelegate OnBeginItemAction;
	FEndItemActionDelegate OnEndItemAction;
	FItemActionUpdateDelegate OnItemActionUpdate;

	UFUNCTION()
	bool IsUsingItem() const;
	bool IsInItemAction() const;
	void StopItemAction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Attack;

public:
	int32 comboCnt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float comboMinTime = 1.0f;
	float comboMaxTime = 1.4f;
	float comboCurrTime = 0;

public:
	UPROPERTY(Replicated)
	int32 KnifeCount = 0;

public:
	void RogueAttack();
	UFUNCTION(Server, Reliable)
	void ServerRPC_RogueAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_RogueAttack();

	void UpdateComboCount(float DeltaTime);

public:
	void RogueThrowKnifeAttack();

	UFUNCTION(Server, Reliable)
	void ServerRPC_RogueThrowKnifeAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_RogueThrowKnifeAttack(bool isSpawn);

protected:
	void UseItem();
	UFUNCTION(Server, Reliable)
	void Server_UseItem();

	UFUNCTION(Server, Reliable)
	void Server_StopItemAction();

	//Only runs on server
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartMontage();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopMontage();

private:
	FScriptDelegate Delegate;

	UFUNCTION()
	void OnRep_bItemActionStarted();
	UPROPERTY(Replicated)
	bool bUsingItem;

	UPROPERTY(ReplicatedUsing = "OnRep_bItemActionStarted")
	bool bItemActionStarted;

	void ItemActionUpdate(float DeltaTime);

	UFUNCTION()
	void OnRep_ItemActionTime();
	UPROPERTY(ReplicatedUsing = "OnRep_ItemActionTime")
	FItemActionTime ItemActionTime;
};


