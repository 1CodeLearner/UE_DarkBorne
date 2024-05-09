// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/AnimMorigeshEnemy.h"

void UAnimMorigeshEnemy::AnimNotify_Start_Hit()
{
	isHitting = true;
}

void UAnimMorigeshEnemy::AnimNotify_End_Hit()
{
	UE_LOG(LogTemp,Warning,TEXT("EndHitEnemey"));
	isHitting = false;
}
