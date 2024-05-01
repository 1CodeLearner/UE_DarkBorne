// Fill out your copyright notice in the Description page of Project Settings.


#include "DBInteractionComponent.h"
#include "../../DBCharacters/DBCharacter.h"
#include "GameFramework/PlayerController.h"

UDBInteractionComponent::UDBInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractDistance = 2000.f;
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
		ObjectQueryparams.AddObjectTypesToQuery(ECC_WorldStatic);
		//ObjectQueryparams.AddObjectTypesToQuery(ECC_GameTraceChannel2);
		//ObjectQueryparams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
		FCollisionQueryParams QueryParams;
		FCollisionShape Shape;
		Shape.Box;

		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.f);
		QueryParams.AddIgnoredActor(Character);
		bool bHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryparams, Shape, QueryParams);

		if (bHit)
		{
			FHitResult finalHit;
			for (auto Hit : Hits) {
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(Character);
				FHitResult hit;
				GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECC_Visibility, Params);
				if (ensureAlways(hit.bBlockingHit)) {
					float what = FVector::DistSquared(Hit.ImpactPoint, hit.ImpactPoint);
					GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Black, FString::Printf(TEXT("Center:%s, ImpactPoint:%s, Distance: %f"), *hit.ImpactPoint.ToString(), *Hit.ImpactPoint.ToString(), what));
				}


			}
		}
	}
}

