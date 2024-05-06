// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStatusAnimInterface.generated.h"


//enum CharacterAnimationFlag
//{
//	ANIM_IS_HIT = 1 << 0,
//	ANIM_IS_DEAD = 1 << 1
//};

 

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterStatusAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKBORNE_API ICharacterStatusAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	
public:
	virtual void SetAnimation(uint8 value) = 0;
	virtual uint8 GetAnimation() const = 0;


	
};
