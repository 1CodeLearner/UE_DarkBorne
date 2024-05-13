// Fill out your copyright notice in the Description page of Project Settings.


#include "../Status/CharacterStatusComponent.h"
#include "../DBCharacters/DBCharacter.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyFSMBase.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
	
}

// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	MyActor = Cast<ACharacter>(GetOwner());

	// ...
	CurrHP = MaxHP;
	
}


void UCharacterStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterStatusComponent, MyActor);
	DOREPLIFETIME(UCharacterStatusComponent, MaxHP);
	DOREPLIFETIME(UCharacterStatusComponent, CurrHP);
}

// Called every frame
void UCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	// ...
}

void UCharacterStatusComponent::DamageProcess(float damage, AActor* From)
{
	if(CurrHP <= 0) return;
	UE_LOG(LogTemp, Warning, TEXT("DamageProcess"));
	if (!MyActor->HasAuthority() || MyActor == nullptr)
	{
		return;
	}
	
	if (AEnemyBase* enemy = Cast<AEnemyBase>(MyActor))
	{

		UE_LOG(LogTemp, Warning, TEXT("EnemyHit"));
		enemy->DamageProcess(damage, From);

		
	}
	else if (ADBCharacter* player = Cast<ADBCharacter>(MyActor))
	{
		CurrHP -= damage;
		if (CurrHP < 0)
		{
			CurrHP = 0;
		}
	}
	
	
}



void UCharacterStatusComponent::OnRep_CurrHP()
{
	if (GetOwner())
	{
		if (ADBCharacter* player = Cast<ADBCharacter>(GetOwner()))
		{
			player->OnRep_CurrHP();
		}
	}
}

