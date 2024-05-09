// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MorigeshWeapon.generated.h"

UCLASS()
class DARKBORNE_API AMorigeshWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMorigeshWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(Server, Reliable)
	void ServerRPC_OnOverlapBegin(class AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_OnOverlapBegin(class AActor* OtherActor);
public:
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BloodVFX;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite)
	AActor* myActor;
	UPROPERTY(EditAnywhere)
	class USphereComponent* compSphere;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* compProjectile;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float weaponDamage = 5;


public:
	UFUNCTION()
	void AutoDestroy();

};
