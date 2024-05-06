// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatusComponent.generated.h"


//ĳ���� ü�� ����
// ü�� ����
// ���� ������
// 
//ĳ���� ���� ����
// �ӵ� ����
// ����
// ü�� ȸ��
// ��������
//

//���� �þ�� uint64 ��ü

//
DECLARE_DELEGATE_OneParam(AnimationUpdateDelegate,uint8)
DECLARE_MULTICAST_DELEGATE_OneParam(TickUpdateDelegate,float)

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
	
	//�����̻�
	bool RegisterCondition(class UConditionBaseData* condition);
	//�����̻� ����/*
	bool RemoveCondition(class UConditionBaseData* condition);
public:	
	
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsWaitComplete(float delayTime);

	
	
public:
	
	TickUpdateDelegate tickDelegate;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class AActor* myActor;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* skeletal;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	class UAnimInstance* myAnim;
		
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<class UConditionBaseData*> conditionList;


	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    TScriptInterface<class ICharacterStatusAnimInterface> CharacterAnimInterface;
	//���� ����

	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite)
	int MaxHP = 100;
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite)
	int CurrHp = MaxHP;

	/// <summary>
	/// 1 = is hit
	/// 2 = is death
	/// 3
	/// 4
	/// 5
	/// 6
	/// 7
	/// 8
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite)
	uint8 animationstatus = 0b00000000;

};
