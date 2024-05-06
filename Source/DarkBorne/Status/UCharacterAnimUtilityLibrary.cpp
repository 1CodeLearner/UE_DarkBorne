// Fill out your copyright notice in the Description page of Project Settings.


#include "../Status/UCharacterAnimUtilityLibrary.h"


void UUCharacterAnimUtilityLibrary::SetAnimationStatus(uint8& Status, EAnimationFlags Flag)
{
    // 주어진 플래그를 켜기 위해 비트 연산 사용
    Status |= static_cast<uint8>(Flag);
}

void UUCharacterAnimUtilityLibrary::ClearAnimationStatus(uint8& Status, EAnimationFlags Flag)
{
    // 주어진 플래그를 끄기 위해 비트 연산 사용
    Status &= ~static_cast<uint8>(Flag);
}

bool UUCharacterAnimUtilityLibrary::IsAnimationStatusSet(uint8 Status, EAnimationFlags Flag)
{
    // 주어진 플래그가 활성화되어 있는지 확인
    return (Status & static_cast<uint8>(Flag)) != 0;
}