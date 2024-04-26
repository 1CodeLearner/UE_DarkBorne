// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../ItemTypes/ItemType.h"
#include "DBCharacter.generated.h"

class UDataTable;

class UInventoryMainWidget;
class ULootDisplayWidget;

class UDBEquipmentComponent;
class UPlayerEquipmentComponent;
class ULootInventoryComponent;
class ULootEquipmentComponent;

UCLASS()
class DARKBORNE_API ADBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;
public:
	UFUNCTION(BlueprintCallable)
	const FFinalStat& GetFinalStat() const;

	UPROPERTY(EditAnywhere, Category = "Settings")
	UDataTable* DT_CharacterStats;
	UPROPERTY(EditAnywhere, Category = "Settings")
	FName RowName;
	UPROPERTY(BlueprintReadOnly)
	FCharacterBaseStat CharacterBaseStat;
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FFinalStat FinalStat;

	//Inventory Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UDBEquipmentComponent* EquipmentComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UPlayerEquipmentComponent* PlayerEquipmentComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	ULootInventoryComponent* LootInventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	ULootEquipmentComponent* LootEquipmentComponent;

	//Inventory UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UInventoryMainWidget> InvMainWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	UInventoryMainWidget* InvMainWidget;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDBPlayerWidget* PlayerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDBPlayerWidget> PlayerWidgetClass;

public:
	//player Mapping Context
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_DBMapping;

	//player input
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Jump;

public:
	UPROPERTY(Replicated ,EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	// 현재 체력을 계속 업뎃시키는 함수를 replicate 이거는 클라에서만 호출됨
	UPROPERTY(ReplicatedUsing = OnRep_CurrHP, EditAnywhere)
	float CurrHP = MaxHP;

public:
	//FVector newVec;
public:
	//Default 이동 관련 함수들
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedStopJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);

public:
	void CreatePlayerWidget();

	UFUNCTION()
	void OnRep_CurrHP();
};
