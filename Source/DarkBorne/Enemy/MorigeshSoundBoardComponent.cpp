// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkBorne/Enemy/MorigeshSoundBoardComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMorigeshSoundBoardComponent::UMorigeshSoundBoardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMorigeshSoundBoardComponent::BeginPlay()
{
	Super::BeginPlay();
	

	// ...
	
}


// Called every frame
void UMorigeshSoundBoardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMorigeshSoundBoardComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UMorigeshSoundBoardComponent::Local_PlaySoundAtLocation(USoundBase* soundBase, FVector Location)
{
	MultiRPC_PlaySoundAtLocation(soundBase,Location);
}


void UMorigeshSoundBoardComponent::MultiRPC_PlaySoundAtLocation_Implementation(USoundBase* soundBase, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), soundBase, Location);
}

