// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/AnimEnemyBase.h"


void UAnimEnemyBase::AnimNotify_End_Hit()
{
	isHitting = false;
}
