// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

public:
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Q_Skill;

public:
	//은신 머티리얼
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VanishMat;

	
public:
	bool isVanish = false;
	float MaxVanishTime = 5;
	float CurrVanishTime = 0;
public:
	void UpdateRogueQSkill(float DeltaTime);
	void ActiveRogueQSkill();
	void DeactiveRogueQSkill();
};
