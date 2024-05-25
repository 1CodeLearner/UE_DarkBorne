// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

IInteractionInterface::IInteractionInterface()
{
	InteractionTime = 30.f;
}

void IInteractionInterface::SetInteractionTime(float InteractTime)
{
	InteractionTime = InteractTime;
}

float IInteractionInterface::GetInteractionTime() const
{
	return InteractionTime;
}
