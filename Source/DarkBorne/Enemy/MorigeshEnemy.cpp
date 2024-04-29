// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshEnemy.h"
#include "MorigeshEnemyFSM.h"

AMorigeshEnemy::AMorigeshEnemy()
{	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> characterMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/ParagonMorigesh/Characters/Heroes/Morigesh/Meshes/Morigesh.Morigesh'"));
	if(characterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(characterMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88), FRotator(0,-90,0));
		myMesh = GetMesh();
	
	}	

	baseFSM = CreateDefaultSubobject<UMorigeshEnemyFSM>(TEXT("FSM"));
	

	

	


}

void AMorigeshEnemy::BeginPlay()
{
	Super::BeginPlay();



}

void AMorigeshEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMorigeshEnemy::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
