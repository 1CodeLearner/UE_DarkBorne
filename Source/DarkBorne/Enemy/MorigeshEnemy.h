// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Enemy/EnemyBase.h"
#include "MorigeshEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DARKBORNE_API AMorigeshEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AMorigeshEnemy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY()
	class USkeletalMeshComponent* myMesh;

	UPROPERTY()
	class UMorigeshSoundBoardComponent* SoundBoard;

	virtual FDisplayInfo GetDisplayInfo() const override;
};
