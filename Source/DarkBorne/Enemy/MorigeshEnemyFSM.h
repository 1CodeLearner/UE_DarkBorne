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

public:
	
	UPROPERTY(EditAnywhere)
	class UAnimMorigeshEnemy* anim;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* montage;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class AActor> attackProjectileFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMorigeshWeapon> weaponFactory;

	//UPROPERTY(EditAnywhere)
	//TArray<class AActor*> firedWeaponList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnBorder = 100;


public:
	

	virtual void ChangeState(EEnemyState s) override;

	void FireWeapon(FVector targetPos);
	

	
	


};
