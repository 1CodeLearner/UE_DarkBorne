// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildTestComponent.h"

void UChildTestComponent::TestFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm calling from Child"));

}
