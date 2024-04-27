// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Items/Weapons/RogueThrowingKnife.h"
#include "DBRogueSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UDBRogueSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDBRogueSkillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Q_Skill;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_E_Skill;

public:
	//은신 머티리얼
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VanishMat;

public:

	UPROPERTY(EditAnywhere)
	class ARogueThrowingKnife* ThrowingKnife;

	// 수리검 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<ARogueThrowingKnife> ThrowingKnifeClass;

	// 수리검 클래스를 담은 배열
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ARogueThrowingKnife>> ThrowKnifeArray;
public:
	UPROPERTY(Replicated, EditAnywhere)
	bool isVanish = false;
	UPROPERTY(Replicated, EditAnywhere)
	float MaxVanishTime = 5;
	UPROPERTY(Replicated, EditAnywhere)
	float CurrVanishTime = 0;

public:
	UPROPERTY(EditAnywhere)
	bool isSpawnKnife = false;
	
public:
	void UpdateRogueQSkill(float DeltaTime);

	void ActiveRogueQSkill();
	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueQSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueQSkill();

	void DeactiveRogueQSkill();
	
public:
	void ActiveRogueESkill();

	
};
