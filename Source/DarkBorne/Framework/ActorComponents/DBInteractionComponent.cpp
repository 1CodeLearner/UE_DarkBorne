// Fill out your copyright notice in the Description page of Project Settings.


#include "DBInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/InteractionInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "../../DBCharacters/DBCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../../Inventory/InventoryMainWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

UDBInteractionComponent::UDBInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	InteractDistance = 2000.f;
	InteractRadius = 50.f;
	interactSpeed = 3.f;
	bInteracting = false;

	currTime = 0.f;
}

void UDBInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	auto TempCharacter = GetOwner<ACharacter>();
	if (TempCharacter && TempCharacter->IsLocallyControlled())
	{
		Character = TempCharacter;
		DBCharacter = Cast<ADBCharacter>(Character);
	}
}

void UDBInteractionComponent::OnInteract()
{
	if (Character && Character->IsLocallyControlled())
	{
		if (!bInteracting) {
			if (OverlappingActor)
			{
				bInteracting = true;
				IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);

				OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::BEGININTERACT);

				SetCanInteract(OverlappingActor, true);
				Interface->BeginInteract(this);
			}
		}
		else if (bInteracting) {
			bInteracting = false;
			ResetTimer();
			SetCanInteract(OverlappingActor, true);

			OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::INTERRUPTINTERACT);

			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->InterruptInteract();
		}
	}
}

bool UDBInteractionComponent::IsDead()
{
	if (!DBCharacter) return false;

	if (DBCharacter->CurrHP <= 0.f)
		return true;

	return false;
}

void UDBInteractionComponent::ExecuteInteraction()
{
	if (bInteracting) {
		bInteracting = false;
		ResetTimer();

		OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::EXECUTEINTERACT);

		IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
		Interface->ExecuteInteract(this, Character);
		Interface->SetCanInteract(true);
	}
}

void UDBInteractionComponent::DeclareFailedInteraction()
{
	if (!bInteracting && OverlappingActor)
	{
		//display fail to pickup message
		/*SetCanInteract(OverlappingActor, true);
		auto Interface = Cast<IInteractionInterface>(OverlappingActor);
		Interface->BeginTrace();
		OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::BEGINTRACE);*/

		ResetTimer();
		bInteracting = false;
		SetCanInteract(OverlappingActor, true);

		OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::BEGINTRACE);

		IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
		Interface->InterruptInteract();
	}
}

void UDBInteractionComponent::SetCanInteract(AActor* InteractingActor, bool bCanInteract)
{
	IInteractionInterface* Interface = Cast<IInteractionInterface>(InteractingActor);
	if (Interface)
		Interface->SetCanInteract(bCanInteract);

	if (!GetOwner()->HasAuthority())
		Server_SetCanInteract(InteractingActor, bCanInteract);
}

void UDBInteractionComponent::Server_SetCanInteract_Implementation(AActor* InteractingActor, bool bCanInteract)
{
	SetCanInteract(InteractingActor, bCanInteract);
}

void UDBInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	if (!Character)
		return;
	if (!Character->IsLocallyControlled())
		return;
	//Check if player is dead, stop ticking and return if true
	if (IsDead())
	{
		OnInteractActorUpdate.ExecuteIfBound(nullptr, EInteractState::ENDTRACE);

		IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
		if (Interface)
			Interface->EndTrace();

		OverlappingActor = nullptr;

		SetComponentTickEnabled(false);
		return;
	}

	if (bInteracting) {
		UpdateTimer(DeltaTime, bDebugDraw);
	}
	else if (CanTrace(bDebugDraw))
		UpdateOverlappingActor(bDebugDraw);

	if (bDebugDraw)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("ActorTraced: %s"), *GetNameSafe(OverlappingActor)));
	}
}

bool UDBInteractionComponent::CanTrace(bool bDebugDraw)
{
	if (!Character) return false;

	bool bCanTrace = true;

	FVector Vel = Character->GetMovementComponent()->Velocity;
	float dotForward = FVector::DotProduct(Vel, Character->GetActorLocation());
	if (bDebugDraw)
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Dot:%f"), dotForward));
	if (abs(dotForward) != 0.f)
		bCanTrace = false;

	//Change this to more efficient code
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UInventoryMainWidget::StaticClass());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("WidgetCount :%d"), Widgets.Num()));
	if (Widgets.Num() > 0)
		bCanTrace = false;

	if (!bCanTrace)
	{
		if (OverlappingActor)
		{
			OnInteractActorUpdate.ExecuteIfBound(nullptr, EInteractState::ENDTRACE);
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->EndTrace();
			OverlappingActor = nullptr;
		}
		return false;
	}

	return true;
}

void UDBInteractionComponent::UpdateOverlappingActor(bool bDebugDraw)
{
	TArray<FHitResult> Hits;
	if (ensureAlways(Character) && Character->IsLocallyControlled())
	{
		FVector Start = Character->GetPawnViewLocation();
		FVector End = Start + GetOwner()->GetActorForwardVector() * InteractDistance;

		auto PC = Character->GetController<APlayerController>();
		if (PC)
		{
			int32 ViewportSizeX;
			int32 ViewportSizeY;
			PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
			FVector2D ScreenLoc = FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);
			FVector WorldLoc;
			FVector WorldDir;
			if (PC->DeprojectScreenPositionToWorld(ScreenLoc.X, ScreenLoc.Y, WorldLoc, WorldDir))
			{
				Start = WorldLoc;
				End = Start + WorldDir * InteractDistance;
			}
		}

		FCollisionObjectQueryParams ObjectQueryparams;
		//ObjectQueryparams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryparams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
		ObjectQueryparams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
		FCollisionQueryParams QueryParams;
		FCollisionShape Shape;
		Shape.SetSphere(InteractRadius);

		if (bDebugDraw)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.f);
		}

		QueryParams.AddIgnoredActor(Character);
		bool bHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryparams, Shape, QueryParams);

		if (bHit)
		{
			for (auto Hit : Hits)
			{
				if (bDebugDraw)
				{
					DrawDebugSphere(GetWorld(), Hit.ImpactPoint, InteractRadius, 32, FColor::Blue, false, 0.0f);
				}

				AActor* HitActor = Hit.GetActor();
				if (HitActor && HitActor->Implements<UInteractionInterface>())
				{
					if (!Cast<IInteractionInterface>(HitActor)->CanInteract())
					{
						if (OverlappingActor) {
							IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
							Interface->EndTrace();
							OverlappingActor = nullptr;
							OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::ENDTRACE);
						}
						break;
					};

					if (!OverlappingActor)
					{
						OverlappingActor = HitActor;

						auto Interface = Cast<IInteractionInterface>(OverlappingActor);
						Interface->BeginTrace();
						OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::BEGINTRACE);
					}
					else if (OverlappingActor != HitActor)
					{
						IInteractionInterface* prevActor = Cast<IInteractionInterface>(OverlappingActor);
						prevActor->EndTrace();
						OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::ENDTRACE);

						OverlappingActor = HitActor;

						IInteractionInterface* currActor = Cast<IInteractionInterface>(OverlappingActor);
						currActor->BeginTrace();
						OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::BEGINTRACE);
					}
				}
				break;
			}
		}
		else if (OverlappingActor) {
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->EndTrace();
			OverlappingActor = nullptr;
			OnInteractActorUpdate.ExecuteIfBound(OverlappingActor, EInteractState::ENDTRACE);
		}
	}
}

void UDBInteractionComponent::UpdateTimer(float DeltaTime, bool bDebugDraw)
{
	if (bInteracting) {
		currTime += DeltaTime;

		OnInteractTimeUpdate.ExecuteIfBound(currTime, interactSpeed);

		if (currTime >= interactSpeed)
		{
			ExecuteInteraction();
		}

		if (bDebugDraw)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(
				TEXT("UpdateTimer: %0.2f"), currTime));
		}
	}
}

void UDBInteractionComponent::ResetTimer()
{
	currTime = 0.f;
}

