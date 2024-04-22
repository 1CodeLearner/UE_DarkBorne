// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBRogueAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UDBRogueAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBRogueAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);


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
	void RogueAttack();
	UFUNCTION(Server, Reliable)
	void ServerRPC_RogueAttack();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_RogueAttack();

	void UpdateComboCount(float DeltaTime);
	
};
