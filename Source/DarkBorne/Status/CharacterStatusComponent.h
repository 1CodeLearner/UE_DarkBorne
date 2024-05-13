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
	// ���� ü���� ��� ������Ű�� �Լ��� replicate �̰Ŵ� Ŭ�󿡼��� ȣ���
	UPROPERTY(Replicated,ReplicatedUsing = OnRep_CurrHP, EditAnywhere)
	float CurrHP = MaxHP;
};
