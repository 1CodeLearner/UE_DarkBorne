// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../ItemTypes/ItemType.h"
#include "DBCharacter.generated.h"

class UDataTable;

class UInventoryMainWidget;
class UDBEquipmentComponent;
class UPlayerEquipmentComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UDBEquipmentComponent* EquipmentComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UPlayerEquipmentComponent* PlayerEquipmentComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UInventoryMainWidget> InvMainWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	UInventoryMainWidget* InvMainWidget;


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
	//Default 이동 관련 함수들
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedStopJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);
};
