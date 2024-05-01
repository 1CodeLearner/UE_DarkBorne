// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "../Framework/ActorComponents/DBInteractionComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>
#include "../Framework/Interfaces/InteractionInterface.h"
#include "../Inventory/ItemObject.h"

void UInteractWidget::DisplayInteract(FString NameStr, FString ActionStr)
{
	SetNameText(NameStr);
	SetActionText(ActionStr);
}

void UInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APawn* What = GetOwningPlayerPawn();
	
	auto Comp = What->GetComponentByClass<UDBInteractionComponent>();
		
	Comp->OnInteractActorUpdate.BindUObject(this, &UInteractWidget::OnInteractActorUpdate);
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractWidget::SetNameText(FString NameStr)
{
	Text_Name->SetText(FText::FromString(NameStr));
}

void UInteractWidget::SetActionText(FString ActionStr)
{
	Text_Action->SetText(FText::FromString(ActionStr));
}

void UInteractWidget::OnInteractActorUpdate(AActor* ActorFound)
{
	if (ActorFound) {
		SetVisibility(ESlateVisibility::HitTestInvisible);

		auto Interact = Cast<IInteractionInterface>(ActorFound);
		FDisplayInfo displayInfo = Interact->GetDisplayInfo();
		DisplayInteract(displayInfo.Name, displayInfo.Action);
		
		UE_LOG(LogTemp, Warning, TEXT("OnInteractActorFound here, %f"), 10.f);
	}
	else {
		SetVisibility(ESlateVisibility::Collapsed);
	}

}

