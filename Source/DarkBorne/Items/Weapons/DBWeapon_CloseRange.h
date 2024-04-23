// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DBWeapon.h"
#include "DBWeapon_CloseRange.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API ADBWeapon_CloseRange : public ADBWeapon
{
	GENERATED_BODY()

public:
	ADBWeapon_CloseRange();

public:
	virtual void BeginPlay() override;

public:
	//�ݸ��� �Լ�
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION(NetMulticast, Reliable)
	//void MultiRPC_OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BloodVFX;
	//���� ���� �Լ�
	
	//��Ƽĳ��Ʈ �Լ�
};
