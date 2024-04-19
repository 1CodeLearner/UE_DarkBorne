// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"


FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // 예제 값으로 100x100 설정
	return _dimentions;
}

UMaterial* UItemObject::GetIcon()
{
	return Icon;
}

TSubclassOf<AActor> UItemObject::GetItemClass()
{
	return ItemClass;
}

UWorld* UItemObject::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}