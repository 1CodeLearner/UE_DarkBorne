// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBCharacters/DBRogueCharacter.h"
#include "../ItemTypes/ItemType.h"
#include "J_TestCharacter.generated.h"

/**
 *
 */

class UDataTable;
class UPlayerEquipmentComponent;
class UInventoryMainWidget;

UCLASS()
class DARKBORNE_API AJ_TestCharacter : public ADBRogueCharacter
{
	GENERATED_BODY()
public:
	AJ_TestCharacter();
	
protected:
	virtual void BeginPlay() override;
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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UPlayerEquipmentComponent* PlayerEquipmentComp;

	UPROPERTY(EditDefaultsOnly, Category= "Settings")
	TSubclassOf<UInventoryMainWidget> InvMainWidgetClass;

	UPROPERTY(VisibleAnywhere, Category= "Settings")
	UInventoryMainWidget* InvMainWidget;
};

