// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ItemTypes/EnchantmentTypes.h"
#include "CharacterStatusComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStatusComponent();

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


	UFUNCTION()
	void OnRep_CurrHP();

public:
	UPROPERTY(Replicated, EditAnywhere)
	ACharacter* MyActor;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	// 현재 체력을 계속 업뎃시키는 함수를 replicate 이거는 클라에서만 호출됨
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrHP, EditAnywhere)
	float CurrHP = MaxHP;


//Character Stats
public:
	void Initialize();

	UFUNCTION(BlueprintCallable)
	const FAddedStat& GetAddedStat() const;
	UFUNCTION(BlueprintCallable)
	const FBaseStat& GetBaseStat() const;

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
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FBaseStat BaseStat;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FAddedStat AddedStat;
	bool bInitialized;
		
	void AddStats(const UItemObject* ItemObject);
	void RemoveStats(const UItemObject* ItemObject);

	void PrintStats();
};
