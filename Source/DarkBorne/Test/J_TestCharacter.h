// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../DBCharacters/DBCharacter.h"
#include "../ItemTypes/ItemType.h"
#include "J_TestCharacter.generated.h"

/**
 *
 */

class UDataTable;

UCLASS()
class DARKBORNE_API AJ_TestCharacter : public ADBCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:

	UFUNCTION(BlueprintCallable)
	const FFinalStat& GetFinalStat() const;

	UPROPERTY(EditAnywhere, Category="Settings")
	UDataTable* DT_CharacterStats;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName RowName;

	UPROPERTY(BlueprintReadOnly)
	FCharacterBaseStat CharacterBaseStat;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FFinalStat FinalStat;
};
