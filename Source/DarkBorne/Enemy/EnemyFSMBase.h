// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSMBase.generated.h"


//공통 Enemy Locomotion 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	PATROL,
	ATTACK,
	ATTACK_DELAY,
	DAMAGE,
	DIE
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UEnemyFSMBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSMBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//AIController
	UPROPERTY(EditAnywhere)
	class AAIController* ai;
	



	EEnemyState currState = EEnemyState::IDLE;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class ADBCharacter* target;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class AEnemyBase* myActor;


	


		
};
