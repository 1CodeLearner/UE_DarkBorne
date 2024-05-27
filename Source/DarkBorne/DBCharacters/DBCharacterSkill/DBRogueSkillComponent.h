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
	//���� ��Ƽ����
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* VanishMat;
public:
	UPROPERTY()
	class ARogueThrowingKnife* ThrowingKnife;

	// �����˵��� ��� ������ źâ
	UPROPERTY(Replicated ,VisibleAnywhere)
	TArray<class ARogueThrowingKnife*> TKMagazine;

	// ������ Ŭ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARogueThrowingKnife> ThrowingKnifeClass;

public:
	//������ ���θ� �Ǵ�
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isVanish = false;
	// ���� �ִ�, ����ð�
	UPROPERTY(Replicated, VisibleAnywhere)
	float MaxVanishTime = 5;
	UPROPERTY(Replicated, VisibleAnywhere)
	float CurrVanishTime = 0;
	// ���� �ִ�, ���� ��Ÿ��
	UPROPERTY(Replicated, VisibleAnywhere)
	float Q_MaxCoolTime = 3;
	UPROPERTY(ReplicatedUsing = OnRep_CurrQSkill, VisibleAnywhere)
	float Q_CurrCoolTime = 0;
	//��ųUI ������Ʈ�� ���� �Լ�
	UFUNCTION()
	void OnRep_CurrQSkill();
	// ���� ���� SFX
	UPROPERTY(EditAnywhere)
	class USoundBase* VanishStartSFX;

public:
	//�������� ��ȯ�Ǿ����� ���θ� �Ǵ�
	UPROPERTY(Replicated, VisibleAnywhere)
	bool isSpawnKnife = false;
	//������ źâ(TKMagazine)�� �ִ� ũ�⸦ ���Ѵ�
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 magazineCnt = 4;
	// ������ ��ų �ִ�, ���� ��Ÿ��
	UPROPERTY(Replicated, VisibleAnywhere)
	float E_MaxCoolTime = 3;
	UPROPERTY(ReplicatedUsing = OnRep_CurrESkill, VisibleAnywhere)
	float E_CurrCoolTime = 0;
	// ������ ��ų UI ������ ���� �Լ�
	UFUNCTION()
	void OnRep_CurrESkill();
public:
	// Q��ų 
	void UpdateRogueQSkill(float DeltaTime);

	void ActiveRogueQSkill();
	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueQSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueQSkill();

	void DeactiveRogueQSkill();
public:
	// E��ų
	void ActiveRogueESkill();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueESkill();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueESkill(bool isSpawn);

	void UpdateRogueESkill(float DeltaTime);
public:
	// Shift ��ų
	void ActiveRogueShiftSkill();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActiveRogueShiftSkill();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ActiveRogueShiftSkill();

public:
	// ����Ʈ ��ų ��Ÿ��
	UPROPERTY(EditAnywhere)
	class UAnimMontage* AM_RogueShiftSkill;

	UPROPERTY()
	ADBCharacter* MyCharacter;
};
