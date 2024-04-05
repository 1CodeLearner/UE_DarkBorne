// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAnimInstance.h"
#include "../DBCharacters/DBRogueCharacter.h"

void UDBRogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player 가져오자
	player = Cast<ADBRogueCharacter>(TryGetPawnOwner());
	
	
}

void UDBRogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player)
	{
		// 앞 뒤 (dirV)
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());

		// 좌 우 (dirH)
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -player->GetBaseAimRotation().Pitch;

		pitchAngle = -player->GetBaseAimRotation().GetNormalized().Pitch;

		// 앞 뒤가 0보다 크거나 좌우가 0보다 크면
		if (dirH > 0 || dirV > 0)
		{
			ShouldMove = true;
		}
		else if(dirH == 0 && dirV == 0)
		{
			ShouldMove = false;
		}
		//else if (dirH < 0 || dirV < 0)
		//{
		//	ShouldMove = true;
		//}
	}
}
