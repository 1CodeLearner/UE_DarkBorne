// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/EnemyFSMBase.h"
//#include <../../../../../../../Source/Runtime/AIModule/Classes/AIController.h>

// FSM���� �κ� �߰� �߻��� �߰��� ��ũ��Ʈ
//������ ������ ����

// Sets default values for this component's properties
UEnemyFSMBase::UEnemyFSMBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSMBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyFSMBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyFSMBase::ChangeState(EEnemyState e)
{
	currState = e;
}

