// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Items/Weapons/RogueThrowingKnife.h"
#include "DBRogueSkillComponent.generated.h"

class ADBCharacter;

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

	// 수리검들을 담는 수리검 탄창
	UPROPERTY(Replicated ,VisibleAnywhere)
	TArray<class ARogueThrowingKnife*> TKMagazine;

	// 수리검 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARogueThrowingKnife> ThrowingKnifeClass;

public:
	//은신의 여부를 판단
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isVanish = false;
	// 은신 최대, 현재시간
	UPROPERTY(Replicated, VisibleAnywhere)
	float MaxVanishTime = 5;
	UPROPERTY(Replicated, VisibleAnywhere)
	float CurrVanishTime = 0;
	// 은신 최대, 현재 쿨타임
	UPROPERTY(Replicated, VisibleAnywhere)
	float Q_MaxCoolTime = 3;
	UPROPERTY(ReplicatedUsing = OnRep_CurrQSkill, VisibleAnywhere)
	float Q_CurrCoolTime = 0;
	//스킬UI 업데이트를 위한 함수
	UFUNCTION()
	void OnRep_CurrQSkill();
	// 은신 시작 SFX
	UPROPERTY(EditAnywhere)
	class USoundBase* VanishStartSFX;

public:
	//수리검이 소환되었는지 여부를 판단
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isSpawnKnife = false;
	//수리검 탄창(TKMagazine)의 최대 크기를 정한다
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 magazineCnt = 4;
	// 수리검 스킬 최대, 현재 쿨타임
	UPROPERTY(Replicated, VisibleAnywhere)
	float E_MaxCoolTime = 3;
	UPROPERTY(ReplicatedUsing = OnRep_CurrESkill, VisibleAnywhere)
	float E_CurrCoolTime = 0;
	// 수리검 스킬 UI 갱신을 위한 함수
	UFUNCTION()
	void OnRep_CurrESkill();
public:
	// Q스킬 
	void UpdateRogueQSkill(float DeltaTime);

	void ActiveRogueQSkill();
	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueQSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueQSkill();

	void DeactiveRogueQSkill();
public:
	// E스킬
	void ActiveRogueESkill();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueESkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueESkill(bool isSpawn);

	void UpdateRogueESkill(float DeltaTime);
public:
	// Shift 스킬
	void ActiveRogueShiftSkill();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueShiftSkill();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueShiftSkill();

public:
	// 쉬프트 스킬 몽타지
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_RogueShiftSkill;

	UPROPERTY()
	ADBCharacter* MyCharacter;
};
