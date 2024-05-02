// Fill out your copyright notice in the Description page of Project Settings.


#include "DBInteractionComponent.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/InteractionInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

UDBInteractionComponent::UDBInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractDistance = 2000.f;
	InteractRadius = 50.f;
	interactSpeed = 3.f;
	bInteracting = false;
}

void UDBInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	auto TempCharacter = GetOwner<ACharacter>();
	if (TempCharacter && TempCharacter->IsLocallyControlled())
		Character = TempCharacter;
}

void UDBInteractionComponent::OnInteract(bool bIsInput)
{
	if (bIsInput)
	{
		if (OverlappingActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnInteract %s"),
				GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server")
			);
			bInteracting = true;
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->Execute_BeginInteract(OverlappingActor, this);
		}
	}
	else if (bInteracting) {
		bInteracting = false;
		IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
		Interface->Execute_InterruptInteract(OverlappingActor);
	}
}

void UDBInteractionComponent::InteractExecute()
{
	if (bInteracting) {
		UE_LOG(LogTemp, Warning, TEXT("Middle %s"),
			GetWorld()->GetNetMode() == NM_Client ? TEXT("Client") : TEXT("Server")
		);
		bInteracting = false;

		IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
		Interface->Execute_ExecuteInteract(OverlappingActor, Character);
	}
}

void UDBInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Dot:%f"), bInteracting ? TEXT("INTERACTING") : TEXT("NOT INTERACTING")));


	if (bInteracting)
		UpdateTimer();
	else if (CanInteract(bDebugDraw))
		UpdateOverlappingActor(bDebugDraw);
}

bool UDBInteractionComponent::CanInteract(bool bDebugDraw)
{
	if (!Character) {
		return false;
	}

	FVector Vel = Character->GetMovementComponent()->Velocity;
	float dotForward = FVector::DotProduct(Vel, Character->GetActorLocation());

	if (bDebugDraw)
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Dot:%f"), dotForward));

	if (abs(dotForward) != 0.f)
	{
		if (OverlappingActor)
		{
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->EndTrace();
			OverlappingActor = nullptr;
			OnInteractActorUpdate.ExecuteIfBound(nullptr);
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
					if (!Cast<IInteractionInterface>(HitActor)->CanInteract()) break;

					if (!OverlappingActor)
					{
						OverlappingActor = HitActor;

						auto Interface = Cast<IInteractionInterface>(OverlappingActor);
						Interface->BeginTrace();
						OnInteractActorUpdate.ExecuteIfBound(OverlappingActor);
					}
					else if (OverlappingActor != HitActor)
					{
						IInteractionInterface* prevActor = Cast<IInteractionInterface>(OverlappingActor);
						prevActor->EndTrace();

						OverlappingActor = HitActor;

						IInteractionInterface* currActor = Cast<IInteractionInterface>(OverlappingActor);
						currActor->BeginTrace();
						OnInteractActorUpdate.ExecuteIfBound(OverlappingActor);
					}
				}
				break;
			}
		}
		else if (OverlappingActor) {
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->EndTrace();
			OverlappingActor = nullptr;
			OnInteractActorUpdate.ExecuteIfBound(OverlappingActor);
		}
	}
}

void UDBInteractionComponent::UpdateTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("UpdateTimer")));

}

