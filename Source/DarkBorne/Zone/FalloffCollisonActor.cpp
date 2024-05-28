// Fill out your copyright notice in the Description page of Project Settings.

#include "DarkBorne/Status/CharacterStatusComponent.h"
#include "DarkBorne/Zone/FalloffCollisonActor.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "DarkBorne/DBCharacters/DBRogueCharacter.h"
#include <DarkBorne/TP_ThirdPerson/TP_ThirdPersonGameMode.h>

// Sets default values
AFalloffCollisonActor::AFalloffCollisonActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FallBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FallBox"));
	FallBoxCollision->SetBoxExtent(FVector(300,300,30));
	FallBoxCollision->SetCollisionProfileName(TEXT("WeaponCapColl"));



}

// Called when the game starts or when spawned
void AFalloffCollisonActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FallBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFalloffCollisonActor::OnOverlapBegin);
	}
	
}

// Called every frame
void AFalloffCollisonActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFalloffCollisonActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherPlayer = Cast<ACharacter>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("Testing here: %s"), *GetNameSafe(GetOwner()));
	//UDBRogueAnimInstance* OtherPlayerAnim = Cast<UDBRogueAnimInstance>(OtherPlayer->GetMesh()->GetAnimInstance());

	// 캐릭터의 GetOnwer로 인스턴스를 가져와 나의 플레이어 애님 인스턴스로 가져온다
	ServerRPC_OnOverlapBegin(OtherActor);
	
}

void AFalloffCollisonActor::ServerRPC_OnOverlapBegin_Implementation(AActor* OtherActor)
{
	//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
	ADBRogueCharacter* OtherPlayer = Cast<ADBRogueCharacter>(OtherActor);
	if (OtherPlayer)
	{
		FString Level = GetWorld()->GetMapName();
		Level.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

		if (Level != TEXT("ThirdPersonMap"))
		{
			UCharacterStatusComponent* StatusComponent = OtherActor->GetComponentByClass<UCharacterStatusComponent>();
			//내가 아닌 다른 로그 플레이어를 otherActor로 캐스팅
			//로비 체크
			StatusComponent->DamageProcess(FallOffDamage, GetOwner());
			//플레이어의 현재 체력에서 무기데미지만큼 데미지를 준다
			//onRep 함수는 클라에서만 호출되어서 서버에서도 한번 호출해줘야한다
			//StatusComponent->OnRep_CurrHP();

			//UE_LOG(LogTemp, Warning, TEXT("%s : %.f"),
			//	GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client") : TEXT("Server"), OtherPlayer->CurrHP);
			auto GM = GetWorld()->GetAuthGameMode<ATP_ThirdPersonGameMode>();
			if (ensure(GM) && StatusComponent->CurrHP <= 0.f && OtherPlayer != nullptr)
			{
				auto PC = OtherPlayer->GetOwner<APlayerController>();
				if (PC)
				{
					GM->OnPlayerDead(PC);
				}
			}
		}
	}
}

