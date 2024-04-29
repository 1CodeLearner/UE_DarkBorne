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
#include <DarkBorne/DBCharacters/DBRogueCharacter.h>


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
	maxMoveTime = 12.f;
	bMove = false;
	currWaitTime = 0.f;
	maxWaitTime = 1.f;
	PhaseCount = -1;

	bIsFirstStarting = true;
	StartingNode = nullptr;
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

		if (Nodes.Num() > 0)
		{
			float distMin = -1.f;
			distMin = GetDistanceFromOrigin(Nodes[0]->GetActorLocation());
			AZoneNode* Holder = Nodes[0];
			for (int i = 1; i < Nodes.Num(); ++i)
			{
				float distance = GetDistanceFromOrigin(Nodes[i]->GetActorLocation());
				if (distance < distMin)
				{
					distMin = distance;
					Holder = Nodes[i];
				}
			}
			StartingNode = Holder;
			UE_LOG(LogTemp, Warning, TEXT("StartingNode:%s"), *GetNameSafe(StartingNode));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("AZoneNodes not found"));
		}

		if (ensureAlways(DT_ZoneSetting)) {
			FZoneSetting* Setting = DT_ZoneSetting->FindRow<FZoneSetting>(FName(TEXT("GoblinCave")), FString::Printf(TEXT("ZoneActor")));
			if (ensureAlways(Setting)) {
				ZoneSetting = Setting;
				UE_LOG(LogTemp, Warning, TEXT("TotalTime:%f"), ZoneSetting->GetTotalTime());
			}
		}

		currScale = GetActorScale3D();
		diffScale = currScale / ZoneSetting->GetPhases().Num();
		UE_LOG(LogTemp, Warning, TEXT("Diff Scale: %s"), *diffScale.ToString());

		auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
		if (GM)
		{
			GM->OnPlayerUpdate.AddUObject(this, &AZoneActor::OnPlayerUpdate);
			GM->OnGameEnd.AddUObject(this, &AZoneActor::OnGameEnd);

			//if the game is running as a listen server, add server player to map container
			TArray<ADBPlayerController*> ConnectedPlayers = GM->GetConnectedPlayers();
			UE_LOG(LogTemp, Warning, TEXT("ConnectedPlayers count: %d"), ConnectedPlayers.Num());

			for (auto PC : ConnectedPlayers) {
				playerOverlapped.Add(PC, false);

				auto ZoneDamage = NewObject<UZoneDamage>(PC);

				float Damage = ZoneSetting->GetPhases()[0].Damage;
				ZoneDamage->Initialize(PC, 2.5f, Damage);
				playerDamaged.Add(PC, ZoneDamage);


				ADBCharacter* Character = PC->GetPawn<ADBCharacter>();
				if (ensureAlways(Character))
					ActiveCharacters.Add(PC, Character);

				PC->OnPossessedPawnChanged.AddDynamic(this, &AZoneActor::OnPossessedPawnChanged);
			}
		}

		maxWaitTime = ZoneSetting->GetPhases()[0].DisplayTime;
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

	if (HasAuthority() && cVarDisplayZoneDebugMsg.GetValueOnGameThread())
	{
		for (auto What : playerOverlapped)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("playerOverlapped: %s"), *GetNameSafe(What.Key)));
		}
		for (auto What : playerDamaged)
		{
			if (IsValid(What.Value))
				GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("playerDamaged: %s, ticking: %s"),
					*GetNameSafe(What.Value),
					What.Value->IsTicking() ? TEXT("TRUE") : TEXT("FALSE")
				));
		}
		for (auto What : ActiveCharacters)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("ActiveCharacters: %s"), *GetNameSafe(What.Value)));
		}

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("currWaitTime %f"), currWaitTime));

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Magenta, FString::Printf(TEXT("currMoveTime %f"), currMoveTime));
	}

	if (HasAuthority() && ensure(CheckSizes()))
	{
		UpdateMovement(DeltaTime);
		UpdatePlayerOverlapped();
		UpdatePlayerDamaged();
	}

}

bool AZoneActor::CheckSizes() const
{
	return ActiveCharacters.Num() == playerOverlapped.Num() &&
		playerOverlapped.Num() == playerDamaged.Num() &&
		ActiveCharacters.Num() == playerDamaged.Num();
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

bool AZoneActor::CanMove() const
{
	return PhaseCount + 1 < ZoneSetting->GetPhases().Num();
}

void AZoneActor::StartMove()
{
	PhaseCount++;

	maxMoveTime = ZoneSetting->GetPhases()[PhaseCount].ShrinkTime;

	for (auto pair : playerDamaged) {
		pair.Value->UpdateDamage(ZoneSetting->GetPhases()[PhaseCount].Damage);
	}

	if (bIsFirstStarting) {
		if (StartingNode) {
			nextLoc = StartingNode->GetActorLocation();
		}
		else {
			nextLoc = GetActorLocation();
		}

		bIsFirstStarting = false;

	}
	else {
		TArray<AZoneNode*> ValidNodes;
		for (int i = 0; i < Nodes.Num(); ++i)
		{
			if (IsWithinZone(Nodes[i]->GetActorLocation())) {
				ValidNodes.Add(Nodes[i]);
			}
		}

		if (ValidNodes.Num() > 0) {
			int index = FMath::RandRange(0, ValidNodes.Num() - 1);
			nextLoc = ValidNodes[index]->GetActorLocation();
		}
		//If there are no valid nodes inside the zone, just shrink without moving
		else nextLoc = GetActorLocation();
	}

	prevLoc = GetActorLocation();

	currScale = GetActorScale3D();
	nextScale = currScale - diffScale;
}

void AZoneActor::UpdatePlayerOverlapped()
{
	for (auto Character : ActiveCharacters) {
		FVector CharacterLoc = Character.Value->GetActorLocation();

		//if player is out of the zone and player was set as within bounds
		if (!IsWithinZone(CharacterLoc) && *playerOverlapped.Find(Character.Key) == false) {
			//Set player as out of bounds
			*playerOverlapped.Find(Character.Key) = true;
		}
		//if player is within the zone and player was set as out of bounds
		else if (IsWithinZone(CharacterLoc) && *playerOverlapped.Find(Character.Key) == true) {
			//Set player as within bounds
			*playerOverlapped.Find(Character.Key) = false;
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

		UZoneDamage** ZoneDamage = playerDamaged.Find(Player);
		if (ZoneDamage)
		{
			(*ZoneDamage)->StopTick();
		}
		playerDamaged.Remove(Player);

		ActiveCharacters.Remove(Player);

	}
	else {

		auto ZoneDamage = NewObject<UZoneDamage>(Player);
		float Damage = ZoneSetting->GetPhases()[std::max(0, PhaseCount)].Damage;
		if (ZoneDamage->Initialize(Player, 2.5f, Damage))
		{
			playerDamaged.Add(Player, ZoneDamage);

			playerOverlapped.Add(Player, false);

			ADBCharacter* Character = Player->GetPawn<ADBCharacter>();
			if (Character)
				ActiveCharacters.Add(Player, Character);
		}
	}
}

void AZoneActor::OnGameEnd(ADBPlayerController* PlayerWon)
{
	playerOverlapped.Remove(PlayerWon);

	auto ZoneDamage = *playerDamaged.Find(PlayerWon);
	ZoneDamage->StopTick();
	playerDamaged.Remove(PlayerWon);
	ActiveCharacters.Remove(PlayerWon);
}

void AZoneActor::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("OldPawn:%s, NewPawn:%s"), *GetNameSafe(OldPawn), *GetNameSafe(NewPawn));

	if (NewPawn)
	{
		auto PC = Cast<ADBPlayerController>(NewPawn->GetOwner());
		if (ensureAlways(PC))
		{
			playerOverlapped.Remove(PC);

			UZoneDamage* ZoneDamage = *playerDamaged.Find(PC);
			if (ensureAlways(ZoneDamage))
			{
				ZoneDamage->StopTick();
			}
			playerDamaged.Remove(PC);

			ActiveCharacters.Remove(PC);
		}
	}
}

void AZoneActor::OnRep_TransformZone()
{
	SetActorLocation(TransformZone.Loc);
	SetActorRelativeScale3D(TransformZone.Scale);
}

bool AZoneActor::IsWithinZone(FVector Other) const
{
	float distSqrd = GetDistanceFromOrigin(Other);
	float radiusSqrd = GetZoneRadius();

	if (HasAuthority() && cVarDisplayZoneDebugMsg.GetValueOnGameThread())
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Emerald, FString::Printf(TEXT("distS:%f, radius:%f"), distSqrd, radiusSqrd));

	return distSqrd < radiusSqrd;
}

float AZoneActor::GetDistanceFromOrigin(FVector OtherLocation) const
{
	OtherLocation.Z = 0.f;
	//float distSqrd = diff.X * diff.X + diff.Y * diff.Y + diff.Z * diff.Z;
	return FVector::Distance(OtherLocation, currLoc);
}

float AZoneActor::GetZoneRadius() const
{
	return CapsuleComp->GetScaledCapsuleRadius();
	//radiusSqrd *= radiusSqrd;
}

float FZoneSetting::GetTotalTime()
{
	if (!bInitialized)
	{
		for (auto What : Phases)
		{
			TotalTime += What.DisplayTime;
			TotalTime += What.ShrinkTime;
		}
		bInitialized = true;
	}

	return TotalTime;
}

const TArray<FZonePhase>& FZoneSetting::GetPhases() const
{
	return Phases;
}
