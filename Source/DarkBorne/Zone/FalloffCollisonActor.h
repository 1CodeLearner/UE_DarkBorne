// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FalloffCollisonActor.generated.h"

UCLASS()
class DARKBORNE_API AFalloffCollisonActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFalloffCollisonActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnOverlapBegin(class AActor* OtherActor);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* FallBoxCollision;

	float FallOffDamage =999;

};
