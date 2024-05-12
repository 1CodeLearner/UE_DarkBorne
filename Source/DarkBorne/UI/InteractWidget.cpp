// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractWidget.h"
#include "Components/TextBlock.h"
#include "../Framework/ActorComponents/DBInteractionComponent.h"
#include "../Framework/Interfaces/InteractionInterface.h"
#include "../Inventory/ItemObject.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "../DBCharacters/DBCharacterAttack/DBRogueAttackComponent.h"
#include "../DBWeapon/DBRogueWeaponComponent.h"
#include "../Framework/Interfaces/ItemInterface.h"
#include "../Items/DBItem.h"

void UInteractWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APawn* Player = GetOwningPlayerPawn();

	auto InteractionComp = Player->GetComponentByClass<UDBInteractionComponent>();
	if (InteractionComp)
	{
		InteractionComp->OnInteractActorUpdate.BindUObject(this, &UInteractWidget::OnInteractActorUpdate);
		InteractionComp->OnInteractTimeUpdate.BindUObject(this, &UInteractWidget::OnInteractTimeUpdate);
	}

	auto AttackComp = Player->GetComponentByClass<UDBRogueAttackComponent>();
	if (AttackComp)
	{
		AttackComp->OnBeginItemAction.BindUObject(this, &UInteractWidget::OnBeginItemAction);
		AttackComp->OnEndItemAction.BindUObject(this, &UInteractWidget::OnEndItemAction);
		AttackComp->OnItemActionUpdate.BindUObject(this, &UInteractWidget::OnInteractTimeUpdate);

	}


	SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractWidget::DisplayBeginTrace(bool bDisplay, AActor* ActorFound)
{
	if (bDisplay)
	{
		if (Canvas_BeginInteract->IsVisible())
			Canvas_BeginInteract->SetVisibility(ESlateVisibility::Collapsed);

		auto Interact = Cast<IInteractionInterface>(ActorFound);
		if (Interact)
		{
			FDisplayInfo displayInfo = Interact->GetDisplayInfo();
			SetNameText(displayInfo.Name);
			SetActionText(displayInfo.Action);
		}
		else {
			SetNameText(FString::Printf(TEXT("None")));
			SetActionText(FString::Printf(TEXT("Error")));
		}

		Canvas_BeginTrace->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		Canvas_BeginTrace->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UInteractWidget::DisplayBeginInteract(bool bDisplay)
{
	if (bDisplay)
	{
		if (Canvas_BeginTrace->IsVisible())
			Canvas_BeginTrace->SetVisibility(ESlateVisibility::Collapsed);

		Canvas_BeginInteract->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		Canvas_BeginInteract->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractWidget::SetNameText(FString NameStr)
{
	Text_Name->SetText(FText::FromString(NameStr));
}

void UInteractWidget::SetActionText(FString ActionStr)
{
	Text_Action->SetText(FText::FromString(ActionStr));
}

void UInteractWidget::OnInteractActorUpdate(AActor* ActorFound, EInteractState InteractState)
{
	switch (InteractState)
	{
	case EInteractState::BEGINTRACE:
	{
		if (ensureAlways(ActorFound))
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
			DisplayBeginTrace(true, ActorFound);
		}
		break;
	}
	case EInteractState::ENDTRACE:
		DisplayBeginTrace(false);
		break;
	case EInteractState::BEGININTERACT:
		DisplayBeginInteract(true);
		break;
	case EInteractState::INTERRUPTINTERACT:
		DisplayBeginInteract(false);
		DisplayBeginTrace(true, ActorFound);
		break;
	case EInteractState::EXECUTEINTERACT:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}

void UInteractWidget::OnInteractTimeUpdate(float CurrentTime, float MaxTime)
{
	ProgressBar_Interact->SetPercent(CurrentTime / MaxTime);
}

void UInteractWidget::OnBeginItemAction()
{
	auto WeaponComp = GetOwningPlayerPawn()->GetComponentByClass<UDBRogueWeaponComponent>();
	if (WeaponComp && ensureAlways(WeaponComp->RogueItems))
	{
		DisplayBeginInteract(true);
	}
}

void UInteractWidget::OnEndItemAction()
{
	DisplayBeginInteract(false);
}

