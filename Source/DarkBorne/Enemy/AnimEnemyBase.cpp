// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/AnimEnemyBase.h"


void UAnimEnemyBase::AnimNotify_End_Hit()
{
	UE_LOG(LogTemp,Warning,TEXT("is end hit"));
	isHitting = false;
}
