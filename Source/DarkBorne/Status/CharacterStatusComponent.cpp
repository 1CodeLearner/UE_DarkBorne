// Fill out your copyright notice in the Description page of Project Settings.


#include "../Status/CharacterStatusComponent.h"
#include "../Status/ConditionBaseData.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterStatusAnimInterface.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;



	// ...
}


// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();


	if(GetOwner() == nullptr) return;
	myActor = GetOwner();

	if(myActor->GetComponentByClass<USkeletalMeshComponent>() == nullptr) return;
	skeletal = myActor->GetComponentByClass<USkeletalMeshComponent>();

	
	if(skeletal->GetAnimInstance() == nullptr) return;
	myAnim = skeletal->GetAnimInstance();

	if (ICharacterStatusAnimInterface* CharacterStatusAnimInterface = Cast<ICharacterStatusAnimInterface>(myAnim))
	{
		CharacterAnimInterface.SetInterface(CharacterStatusAnimInterface);
		CharacterAnimInterface.SetObject(myActor);
	}


}


void UCharacterStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterStatusComponent, myActor);
	DOREPLIFETIME(UCharacterStatusComponent, myAnim);
	DOREPLIFETIME(UCharacterStatusComponent, conditionList);
	DOREPLIFETIME(UCharacterStatusComponent, MaxHP);
	DOREPLIFETIME(UCharacterStatusComponent, CurrHp);
	
}



bool UCharacterStatusComponent::RegisterCondition(class UConditionBaseData* condition)
{

	//if (condition)
	{
		conditionList.Add(condition);
		// ��������Ʈ�� TickUpdate �Լ��� ���ε��մϴ�.
		condition->handler = tickDelegate.AddUObject(condition, &UConditionBaseData::TickUpdate);

		// ������ �迭�� �߰��մϴ�.
		//���� �̺�Ʈ
		condition->StartEvent();
		// ����� ���������� �Ϸ�Ǿ����� ��ȯ�մϴ�.
		return true;
	}
	// ������ ��ȿ���� ���� ��� ���и� ��ȯ�մϴ�.
	return false;
}

bool UCharacterStatusComponent::RemoveCondition(class UConditionBaseData* condition)
{
	if (condition)
	{
		//tickDelegate.Remove()
		//tickDelegate -= condition->TickUpdate(1);
		// ������ �迭���� �����մϴ�.
		
		
		tickDelegate.Remove(condition->handler);
		// ������ �����մϴ�.
		condition->EndEvent();
		condition->ConditionalBeginDestroy();
		conditionList.Remove(condition);
		return true;
	}

	return false;
}

// Called every frame
void UCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	tickDelegate.Broadcast(DeltaTime);

	// ...
}


bool UCharacterStatusComponent::IsWaitComplete(float delayTime)
{	
	float currTime = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("nowcurrTime %f"), currTime));
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delayTime)
	{
		return true;

	}
	return false;
}

////----------------�ӽ����� Stash--------------------/////

/*
//������ ����� ��������̶� ���� ó��
bool UCharacterStatusComponent::RegisterCondition(class UConditionBaseData condition)
{

	//if (condition)
	{
		conditionList.Add(condition);
		UConditionBaseData* nowPoint = &conditionList[conditionList.Num() - 1];
		// ��������Ʈ�� TickUpdate �Լ��� ���ε��մϴ�.
		nowPoint->handler = tickDelegate.AddUObject(nowPoint, &UConditionBaseData::TickUpdate);

		// ������ �迭�� �߰��մϴ�.
		//���� �̺�Ʈ
		nowPoint->StartEvent();
		// ����� ���������� �Ϸ�Ǿ����� ��ȯ�մϴ�.
		return true;
	}
	// ������ ��ȿ���� ���� ��� ���и� ��ȯ�մϴ�.
	return false;
}
*/