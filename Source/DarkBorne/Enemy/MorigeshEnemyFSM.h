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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimMorigeshEnemy* anim;



	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* montage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UMorigeshSoundBoardComponent* SoundBoard;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class AActor> attackProjectileFactory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AMorigeshWeapon> weaponFactory;

	//UPROPERTY(EditAnywhere)
	//TArray<class AActor*> firedWeaponList;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector spawnBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ThrowPos;

	


public:
	
	virtual void ChangeState(EEnemyState s) override;
	UFUNCTION()
	void FireWeapon(FVector targetPos);
	
	virtual void OnRep_CurrentState() override;
	UFUNCTION()
	virtual void UpdateDamaged(float deltaTime);

	/*UFUNCTION(NetMulticast, Reliable)
	virtual void MultiRPC_ChangeState(EEnemyState s) override;*/




	
private:

	bool bIsShuttingDown = false;
	bool IsFireTiming = false;
	bool IsAttackTimerEnabled = false;
};
