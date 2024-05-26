// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionInterface.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

IInteractionInterface::IInteractionInterface()
{
	InteractionTime = 30.f;
	InvenEquipType = EInvenEquipType::Default;
}

void IInteractionInterface::SetInteractionTime(float InteractTime)
{
	InteractionTime = InteractTime;
}

void IInteractionInterface::SetInvenEquipType(EInvenEquipType _InvenEquipType)
{
	InvenEquipType = _InvenEquipType;
}

EInvenEquipType IInteractionInterface::GetInvenEquipType() const
{
	return InvenEquipType;
}

float IInteractionInterface::GetInteractionTime() const
{
	return InteractionTime;
}
