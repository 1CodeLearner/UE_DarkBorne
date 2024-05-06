// Fill out your copyright notice in the Description page of Project Settings.


#include "../Status/UCharacterAnimUtilityLibrary.h"


void UUCharacterAnimUtilityLibrary::SetAnimationStatus(uint8& Status, EAnimationFlags Flag)
{
    // �־��� �÷��׸� �ѱ� ���� ��Ʈ ���� ���
    Status |= static_cast<uint8>(Flag);
}

void UUCharacterAnimUtilityLibrary::ClearAnimationStatus(uint8& Status, EAnimationFlags Flag)
{
    // �־��� �÷��׸� ���� ���� ��Ʈ ���� ���
    Status &= ~static_cast<uint8>(Flag);
}

bool UUCharacterAnimUtilityLibrary::IsAnimationStatusSet(uint8 Status, EAnimationFlags Flag)
{
    // �־��� �÷��װ� Ȱ��ȭ�Ǿ� �ִ��� Ȯ��
    return (Status & static_cast<uint8>(Flag)) != 0;
}