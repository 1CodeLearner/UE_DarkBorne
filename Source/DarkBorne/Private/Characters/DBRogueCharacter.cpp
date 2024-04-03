// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DBRogueCharacter.h"
#include "../Characters/ActorComponents/ChildTestComponent.h"

ADBRogueCharacter::ADBRogueCharacter()
{
	Blah = CreateDefaultSubobject<UChildTestComponent>(TEXT("BlahComp"));
}
