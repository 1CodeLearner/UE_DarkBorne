// Fill out your copyright notice in the Description page of Project Settings.


#include "DBInteractionComponent.h"
#include "../../DBCharacters/DBCharacter.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/InteractionInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

UDBInteractionComponent::UDBInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractDistance = 2000.f;
	InteractRadius = 50.f;
}


void UDBInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	auto TempCharacter = GetOwner<ACharacter>();
	if (TempCharacter && TempCharacter->IsLocallyControlled())
		Character = TempCharacter;
}


void UDBInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

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
					if (!OverlappingActor)
					{
						OverlappingActor = HitActor;

						auto Interface = Cast<IInteractionInterface>(OverlappingActor);
						Interface->Execute_BeginInteract(OverlappingActor, Character);
					}
					else if (OverlappingActor != HitActor)
					{
						IInteractionInterface* prevActor = Cast<IInteractionInterface>(OverlappingActor);
						prevActor->Execute_EndInteract(OverlappingActor);
						
						OverlappingActor = HitActor;

						IInteractionInterface* currActor = Cast<IInteractionInterface>(OverlappingActor);
						currActor->Execute_BeginInteract(OverlappingActor, Character);
					}
				}
				break;
			}
		}
		else if (OverlappingActor) {
			IInteractionInterface* Interface = Cast<IInteractionInterface>(OverlappingActor);
			Interface->Execute_EndInteract(OverlappingActor);
			OverlappingActor = nullptr;
		}
	}
}

