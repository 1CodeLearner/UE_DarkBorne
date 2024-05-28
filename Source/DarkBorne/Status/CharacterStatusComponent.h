// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ItemTypes/EnchantmentTypes.h"
#include "CharacterStatusComponent.generated.h"

class UDBEquipmentComponent;

DECLARE_DELEGATE(FPlayerDeadDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStatusComponent();

	FPlayerDeadDelegate OnPlayerDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//Character Health
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void DamageProcess(float damage, AActor* From = nullptr);

	bool IsAlive() const;

	UFUNCTION()
	void OnRep_CurrHP(float Old);

public:
	UPROPERTY(Replicated, EditAnywhere)
	ACharacter* MyActor;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;
	// 현재 체력을 계속 업뎃시키는 함수를 replicate 이거는 클라에서만 호출됨
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrHP, EditAnywhere,BlueprintReadWrite)
	float CurrHP = MaxHP;


//Character Stats
public:
	void Initialize();

	UFUNCTION(BlueprintCallable)
	const FAddedStat& GetAddedStat() const;
	UFUNCTION(BlueprintCallable)
	const FBaseStat& GetBaseStat() const;
	UFUNCTION(BlueprintCallable)
	FFinalStat GetFinalStat() const;

	UFUNCTION(BlueprintCallable)
	static UCharacterStatusComponent* Get(ADBCharacter* Character);
	UFUNCTION(BlueprintCallable)
	static void AdjustAddedStats(AActor* Instigated, const UItemObject* ItemObject, bool bIsAdd);
	
	void AddBlockAmount(float Amount);
	void RemoveBlockAmount(float Amount);

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataTable* DT_CharacterStats;
	UPROPERTY(EditAnywhere, Category = "Settings")
	FName RowName;

private:
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings")
	FBaseStat BaseStat;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Settings")
	FAddedStat AddedStat;
	bool bInitialized;
		
	void AddStats(const UItemObject* ItemObject);
	void RemoveStats(const UItemObject* ItemObject);
	
	void PrintStats();
};
