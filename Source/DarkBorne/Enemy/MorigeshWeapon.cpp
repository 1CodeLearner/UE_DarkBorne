// Fill out your copyright notice in the Description page of Project Settings.


#include "../Enemy/MorigeshWeapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AMorigeshWeapon::AMorigeshWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// 충돌체

	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	SetRootComponent(compSphere);
	compSphere->SetSphereRadius(13);
	compSphere->SetRelativeLocation(FVector(10,0,0));
	compSphere->SetCollisionProfileName(TEXT("BlockAll"));

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);
	compMesh->SetRelativeRotation(FRotator(-90,0,0));
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Throwing.SM_Throwing'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}

	// 발사체 움직임 관리하는 컴포넌트
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	// 움직일 콤포넌트 설정
	compProjectile->SetUpdatedComponent(compSphere);
	// 초기 속력, 최대 속력
	compProjectile->InitialSpeed = 1500;
	compProjectile->MaxSpeed = 3000;
	// 튕기게 할것인
	compProjectile->bShouldBounce = false;
	// 얼마나 잘 튕기게 할것인지

}

// Called when the game starts or when spawned
void AMorigeshWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(4);
}

// Called every frame
void AMorigeshWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMorigeshWeapon::AutoDestroy()
{

}

