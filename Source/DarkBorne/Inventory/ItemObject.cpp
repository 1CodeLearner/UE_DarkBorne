// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"


FIntPoint UItemObject::GetDimensions()
{
	FIntPoint TempDimensions(100, 100);  // ���� ������ 100x100 ����
	return TempDimensions;
}

UMaterial* UItemObject::GetIcon()
{
	// ��Ƽ������ �ҷ����� ����
	UMaterial* NewMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Path/To/Material.MaterialName"));
	if (!NewMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
	}
	return NewMaterial;
}