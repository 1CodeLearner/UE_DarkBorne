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

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Shift_Skill;

public:
	//은신 머티리얼
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VanishMat;

public:

	UPROPERTY()
	class ARogueThrowingKnife* ThrowingKnife;

	// 수리검 여러개 들고 있는 탄창
	UPROPERTY(Replicated ,VisibleAnywhere)
	TArray<class ARogueThrowingKnife*> TKMagazine;

	// 수리검 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARogueThrowingKnife> ThrowingKnifeClass;

	// 수리검 클래스를 담은 배열
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TArray<TSubclassOf<class ARogueThrowingKnife>> ThrowingKnifeClassArray;
public:
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isVanish = false;
	UPROPERTY(Replicated, VisibleAnywhere)
	float MaxVanishTime = 5;
	UPROPERTY(Replicated, VisibleAnywhere)
	float CurrVanishTime = 0;

public:
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isSpawnKnife = false;

	//수리검 탄창
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 magazineCnt = 4;

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_RogueShiftSkill;
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

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueESkill();

public:
	void ActiveRogueShiftSkill();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueShiftSkill();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueShiftSkill();
};
