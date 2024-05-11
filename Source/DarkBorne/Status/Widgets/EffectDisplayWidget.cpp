// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDisplayWidget.h"
#include "../../Status/DBEffectComponent.h"
#include "../../DBCharacters/DBCharacter.h"
#include "EffectIconWidget.h"
#include "Components/HorizontalBox.h"
#include "../DBEffect.h"

void UEffectDisplayWidget::RemoveEffectIcon(UEffectIconWidget* IconWidget)
{
	HoriBox_EffectIcons->RemoveChild(IconWidget);
}

void UEffectDisplayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto EffectComp = GetOwningPlayerPawn()->GetComponentByClass<UDBEffectComponent>();

	if(EffectComp)
	{
		//EffectComp->OnEffectStart.BindUObject(this, &UEffectDisplayWidget::OnEffectStart);
		EffectComp->OnEffectUpdate.BindUObject(this, &UEffectDisplayWidget::OnEffectUpdate);
	}
}

void UEffectDisplayWidget::OnEffectUpdate(TArray<UDBEffect*> UpdatedEffects)
{
	HoriBox_EffectIcons->ClearChildren();

	for(int32 i = 0; i < UpdatedEffects.Num(); ++i)
	{
		auto IconWidget = CreateWidget<UEffectIconWidget>(GetOwningPlayer(), EffectIconWidget);
		if(IconWidget)
		{
			IconWidget->StartInit(this, UpdatedEffects[i], UpdatedEffects[i]->GetIcon());
			HoriBox_EffectIcons->AddChild(IconWidget);
		}
	}
}

//void UEffectDisplayWidget::OnEffectStart(UDBEffect* effect, UMaterialInterface* IconDisplay)
//{
//	auto IconWidget = CreateWidget<UEffectIconWidget>(GetOwningPlayer(), EffectIconWidget);
//	if(IconWidget)
//	{
//		IconWidget->StartInit(this, effect, IconDisplay);
//	}
//}
