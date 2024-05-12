// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/EnemyFSMBase.h"
#include "MorigeshEnemyFSM.generated.h"

/**
 * 
 */
 
 UENUM(BlueprintType)
enum class EMorigeshState : uint8
{
	IDLE
};
UENUM(BlueprintType)
enum class EMorigashAttackType : uint8 
{
	ATTACK1
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UMorigeshEnemyFSM : public UEnemyFSMBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMorigeshEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(Replicated, EditAnywhere)
	class UAnimMorigeshEnemy* anim;



	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* montage;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class AActor> attackProjectileFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMorigeshWeapon> weaponFactory;

	//UPROPERTY(EditAnywhere)
	//TArray<class AActor*> firedWeaponList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnBorder = 70;


public:
	
	virtual void ChangeState(EEnemyState s) override;
	UFUNCTION()
	void FireWeapon(FVector targetPos);
	
	virtual void OnRep_CurrentState() override;
	UFUNCTION()
	virtual void UpdateDamaged(float deltaTime);
	
	

};
