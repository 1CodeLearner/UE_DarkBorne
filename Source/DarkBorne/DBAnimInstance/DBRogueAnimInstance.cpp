// Fill out your copyright notice in the Description page of Project Settings.


#include "DBRogueAnimInstance.h"
#include "../DBCharacters/DBRogueCharacter.h"

void UDBRogueAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player ��������
	player = Cast<ADBRogueCharacter>(TryGetPawnOwner());
	
	
}

void UDBRogueAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player)
	{
		// �� �� (dirV)
		dirV = FVector::DotProduct(player->GetActorForwardVector(), player->GetVelocity());

		// �� �� (dirH)
		dirH = FVector::DotProduct(player->GetActorRightVector(), player->GetVelocity());

		//0 ~ 360 --> -180 ~ 180
		pitchAngle = -player->GetBaseAimRotation().Pitch;

		pitchAngle = -player->GetBaseAimRotation().GetNormalized().Pitch;

		// �� �ڰ� 0���� ũ�ų� �¿찡 0���� ũ��
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
