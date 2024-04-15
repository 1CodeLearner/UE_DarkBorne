// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "UObject/NoExportTypes.h"


FIntPoint UItemObject::GetDimentions()
{
	//FIntPoint TempDimensions(100, 100);  // ���� ������ 100x100 ����
	return _dimentions;
}

UMaterial* UItemObject::GetIcon()
{
	return Icon;
}

AActor* UItemObject::GetItemClass()
{
	return ItemClass;
}
