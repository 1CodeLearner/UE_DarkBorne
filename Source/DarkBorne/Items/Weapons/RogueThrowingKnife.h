// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBWeapon.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/TimelineComponent.h>
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UPROPERTY(Replicated)

	int32 KnifeNumber = 0;

	UPROPERTY(Replicated)
	float halfValue;

	UPROPERTY(Replicated)
	bool isThrowing = false;
public:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* projectileComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ThrowKnifeTrail;
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ThrowKnifeVFX;

	UPROPERTY(EditAnywhere)
	class USoundBase* ThrowSound;
public:
	//Timeline

	UPROPERTY(EditAnywhere)
	class UCurveFloat* KnifeCurve;

	UPROPERTY(EditAnywhere)
	class UTimelineComponent* KnifeTimeline;

	UFUNCTION()
	void TimelineProgress(float value);

	FTimeline CurveTimeline;

	UPROPERTY()
	FVector StartLoc;
	UPROPERTY()
	FVector EndLoc;
	UPROPERTY(EditAnywhere)
	float ZOffset;

	UPROPERTY()
	FVector TKPosition;

	UPROPERTY()
	FVector TKEndPos;

	UPROPERTY()
	FRotator TKFirstRotation;

public:
	void UpdateKnifeLocation(float DeltaTime);


	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_RogueThrowKnifeAttack();

	
};
