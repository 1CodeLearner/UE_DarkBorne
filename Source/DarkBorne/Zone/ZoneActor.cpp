// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneActor.h"
#include "ZoneNode.h"
#include "ZoneDamage.h"
#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../TP_ThirdPerson/TP_ThirdPersonGameMode.h"
#include "../Framework/DBPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "../DBCharacters/DBCharacter.h"


static TAutoConsoleVariable<bool> cVarStopZoneMovement(TEXT("su.StopZoneMovement"), false, TEXT("Stops Zone from Moving at start"), ECVF_Cheat);

AZoneActor::AZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>("SMComp");
	RootComponent = SMComp;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetRelativeScale3D(FVector((2.27, 2.27, 2.27)));
	CapsuleComp->SetRelativeLocation(FVector(0., 0., 50.));

	currMoveTime = 0.f;
	maxMoveTime = 5.f;
	bMove = false;
	currWaitTime = 0.f;
	maxWaitTime = 1.f;
	index = -1;

}

void AZoneActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		for (TActorIterator<AZoneNode>it(GetWorld()); it; ++it)
		{
			Nodes.Add(*it);
		}

		currScale = GetActorScale3D();
		UE_LOG(LogTemp, Warning, TEXT("scale: %s"), *currScale.ToString());

		diffScale = currScale / Nodes.Num();
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AZoneActor::OnActorOverlap);

		auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
		if (GM)
		{
			//if the game is running as a listen server, add server player to map container
			TArray<ADBPlayerController*> ConnectedPlayers = GM->GetConnectedPlayers();
			UE_LOG(LogTemp, Warning, TEXT("ConnectedPlayers count: %d"), ConnectedPlayers.Num());

			for (auto PC : ConnectedPlayers) {
				playerOverlapped.Add(PC, false);

				auto ZoneDamage = NewObject<UZoneDamage>(this);
				ZoneDamage->Initialize(PC, 5, 10);
				playerDamaged.Add(PC, ZoneDamage);


				ADBCharacter* Character = PC->GetPawn<ADBCharacter>();
				if (ensureAlways(Character))
					ActiveCharacters.Add(Character);
			}
		}
	}
}

void AZoneActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
		if (ensure(GM))
		{
			GM->OnPlayerUpdate.AddUObject(this, &AZoneActor::OnPlayerUpdate);
		}
	}
}

void AZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AZoneActor, TransformZone);
}

void AZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (cVarStopZoneMovement.GetValueOnGameThread())
	{
		return;
	}

	for (auto What : playerOverlapped)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("ZoneActor: %s"), *What.Key->GetName()));
	}
	for (auto What : playerDamaged)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("ZoneActor: %s"), *What.Key->GetName()));
	}

	if (HasAuthority())
	{
		UpdateMovement(DeltaTime);
		UpdatePlayerDamaged();
		UpdatePlayerOverlapped();
	}

}

bool AZoneActor::CanMove() const
{
	return index + 1 < Nodes.Num();
}

void AZoneActor::StartMove()
{
	index++;
	nextLoc = Nodes[index]->GetActorLocation();
	prevLoc = GetActorLocation();

	currScale = GetActorScale3D();
	nextScale = currScale - diffScale;
}

void AZoneActor::UpdateMovement(float DeltaTime)
{
	if (bMove)
	{
		currMoveTime += DeltaTime;
		float delta = currMoveTime / maxMoveTime;
		float X = FMath::Lerp<float, float>(prevLoc.X, nextLoc.X, delta);
		float Y = FMath::Lerp<float, float>(prevLoc.Y, nextLoc.Y, delta);

		//used to check if a given player is within the overlap boundary
		currLoc = FVector{ X, Y, 0 };
		SetActorLocation(FVector{ X, Y, GetActorLocation().Z });

		float scaleX = FMath::Lerp<float, float>(currScale.X, nextScale.X, delta);
		float scaleY = FMath::Lerp<float, float>(currScale.Y, nextScale.Y, delta);
		SetActorScale3D(FVector(scaleX, scaleY, GetActorScale3D().Z));

		if (currMoveTime >= maxMoveTime)
		{
			float finalX = FMath::Lerp<float, float>(prevLoc.X, nextLoc.X, 1.f);
			float finalY = FMath::Lerp<float, float>(prevLoc.Y, nextLoc.Y, 1.f);
			SetActorLocation(FVector{ finalX, finalY, GetActorLocation().Z });

			float fScaleX = FMath::Lerp<float, float>(currScale.X, nextScale.X, 1.f);
			float fScaleY = FMath::Lerp<float, float>(currScale.Y, nextScale.Y, 1.f);
			SetActorScale3D(FVector(fScaleX, fScaleY, GetActorScale3D().Z));

			bMove = false;
			currMoveTime = 0.f;
		}

		//TranformZone is replicated to clients
		TransformZone = FTransformZone{ GetActorLocation(), GetActorRelativeScale3D() };
	}
	else
	{
		currWaitTime += DeltaTime;
		if (currWaitTime >= maxWaitTime)
		{
			if (CanMove())
			{
				bMove = true;
				currWaitTime = 0.f;
				StartMove();
			}
		}
	}
}


void AZoneActor::UpdatePlayerOverlapped()
{
	for (auto Character : ActiveCharacters) {

		FVector CharLoc = Character->GetActorLocation();
		CharLoc.Z = 0.f;
		FVector diff = CharLoc - currLoc;
		//float distSqrd = diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z;
		float distSqrd = FVector::Distance(CharLoc, currLoc);

		float radiusSqrd = CapsuleComp->GetScaledCapsuleRadius();
		//radiusSqrd *= radiusSqrd;

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("distS:%f, radius:%f"), distSqrd, radiusSqrd));

		if (distSqrd > radiusSqrd) {

		}
	}
}

void AZoneActor::UpdatePlayerDamaged()
{
	for (auto pair : playerOverlapped) {
		//if player is overlapping
		if (pair.Value == true) {
			//Find player's zone damage
			UZoneDamage* ZoneDamage = *playerDamaged.Find(pair.Key);
			//Start damaging player
			if (ensureAlways(ZoneDamage) && !ZoneDamage->IsTicking()) {
				ZoneDamage->StartTick();
			}
		}
		else {
			UZoneDamage* ZoneDamage = *playerDamaged.Find(pair.Key);
			//Stop damaging player
			if (ensureAlways(ZoneDamage) && ZoneDamage->IsTicking()) {
				ZoneDamage->StopTick();
			}
		}
	}
}

void AZoneActor::OnPlayerUpdate(ADBPlayerController* Player, bool bExit)
{
	if (bExit)
	{
		playerOverlapped.Remove(Player);

		UZoneDamage* ZoneDamage = *playerDamaged.Find(Player);
		if (ensureAlways(ZoneDamage))
		{
			ZoneDamage->StopTick();
		}
		playerDamaged.Remove(Player);

		ADBCharacter* Character = Player->GetPawn<ADBCharacter>();
		if (ensureAlways(Character))
			ActiveCharacters.Remove(Character);
	}
	else {
		playerOverlapped.Add(Player, false);

		auto ZoneDamage = NewObject<UZoneDamage>(this);
		ZoneDamage->Initialize(Player, 5, 10);
		playerDamaged.Add(Player, ZoneDamage);

		ADBCharacter* Character = Player->GetPawn<ADBCharacter>();
		if (ensureAlways(Character))
			ActiveCharacters.Add(Character);
	}
}

void AZoneActor::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("What %s"), *GetNameSafe(OtherActor));
}

void AZoneActor::OnRep_TransformZone()
{
	SetActorLocation(TransformZone.Loc);
	SetActorRelativeScale3D(TransformZone.Scale);
}


