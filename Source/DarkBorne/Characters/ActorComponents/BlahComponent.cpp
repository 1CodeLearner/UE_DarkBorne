// Fill out your copyright notice in the Description page of Project Settings.


#include "BlahComponent.h"

// Sets default values for this component's properties
UBlahComponent::UBlahComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBlahComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBlahComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBlahComponent::TestFunc()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm calling from parent"));
}

