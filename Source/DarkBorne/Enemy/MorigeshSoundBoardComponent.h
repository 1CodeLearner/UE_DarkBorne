// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MorigeshSoundBoardComponent.generated.h"

//UENUM(BlueprintType)
//enum class EMorigeshSoundType : uint8
//{
//	
//};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKBORNE_API UMorigeshSoundBoardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMorigeshSoundBoardComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void Local_PlaySoundAtLocation(USoundBase* soundBase, FVector Location);


	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_PlaySoundAtLocation(USoundBase* soundBase, FVector Location);
};
