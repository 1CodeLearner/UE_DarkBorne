// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DBItem.generated.h"

USTRUCT()
struct FItemStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float MoveSlowRate;
};

class UStaticMeshComponent;

UCLASS()
class DARKBORNE_API ADBItem : public AActor
{
	GENERATED_BODY()

public:
	ADBItem();


	/*
	��Ÿ�� �÷��� �ϰ� ���� �ɸ��͸� �Ű������� �ִ´�.
	if montage successfully plays, returns true. Returns false otherwise.
	*/
	UFUNCTION(BlueprintCallable)
	bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName);

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimMontage;

private:
	FName Id;
	FItemStat ItemStat;
};
