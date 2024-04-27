// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBWeapon.h"
#include "RogueThrowingKnife.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API ARogueThrowingKnife : public ADBWeapon
{
	GENERATED_BODY()
public:
	ARogueThrowingKnife();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	//콜리전 함수
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnOverlapBegin(class AActor* OtherActor);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_OnOverlapBegin(class AActor* OtherActor);

public:
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BloodVFX;
public:
	int32 KnifeNumber = 0;
	float halfValue;

	
public:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* projectileComponent;

	//UPROPERTY(EditAnywhere)
	//class UArrowComponent* 
	
public:
	void UpdateKnifeLocation();
};
