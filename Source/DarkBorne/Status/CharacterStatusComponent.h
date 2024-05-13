// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void DamageProcess(float damage, AActor* From = nullptr);


	UFUNCTION()
	void OnRep_CurrHP();
		

public:
	UPROPERTY(Replicated, EditAnywhere)
	ACharacter* MyActor;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100;
	// 현재 체력을 계속 업뎃시키는 함수를 replicate 이거는 클라에서만 호출됨
	UPROPERTY(Replicated,ReplicatedUsing = OnRep_CurrHP, EditAnywhere)
	float CurrHP = MaxHP;
};
