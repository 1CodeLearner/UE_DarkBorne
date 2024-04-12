// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"


FIntPoint UItemObject::GetDimensions()
{
	FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	return TempDimensions;
}

UMaterial* UItemObject::GetIcon()
{
	// 머티리얼을 불러오는 예제
	UMaterial* NewMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Path/To/Material.MaterialName"));
	if (!NewMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load material."));
	}
	return NewMaterial;
}