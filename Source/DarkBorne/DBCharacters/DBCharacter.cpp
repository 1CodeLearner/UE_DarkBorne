// Fill out your copyright notice in the Description page of Project Settings.


#include "DBCharacter.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>

#include "../Inventory/InventoryMainWidget.h"
#include "../DBAnimInstance/DBRogueAnimInstance.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/NavMovementComponent.h>

#include "../Inventory/PlayerEquipmentComponent.h"
#include "../Inventory/DBEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include "../DBPlayerWidget/DBPlayerWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/ArrowComponent.h>
#include "DBRogueCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Controller.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "../Framework/ActorComponents/DBInteractionComponent.h"


FFinalStat ADBCharacter::GetFinalStat(ACharacter* Character)
{
	if (Character->IsA<ADBCharacter>()) {
		return Cast<ADBCharacter>(Character)->GetFinalStat();
	}
	return FFinalStat();
}

// Sets default values
ADBCharacter::ADBCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	EquipmentComponent = CreateDefaultSubobject<UDBEquipmentComponent>("EquipmentComp");
	PlayerEquipmentComp = CreateDefaultSubobject<UPlayerEquipmentComponent>("PlayerEquipmentComp");

	GetMesh()->SetCollisionProfileName(TEXT("PlayerMeshColl"));
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerColl"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	InteractDistance = 400.f;
	InteractionComp = CreateDefaultSubobject<UDBInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ProjectileSpawnPos->GetComponentLocation()
	AActor* actor = GetOwner();
	// �� ���̶�� 
	if (IsLocallyControlled())
	{
		//get APlayerController
		APlayerController* playerContoller = Cast<APlayerController>(GetController());

		if (playerContoller == nullptr) return;
		//get subSystem
		UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());


		//����ý����� �����Դٸ�
		if (subSystem)
		{
			subSystem->AddMappingContext(imc_DBMapping, 0);
		}
		//Ŭ����� ���⼭ ��� ������ �����Ѵ�
		if (!HasAuthority())
		{
			CreatePlayerWidget();
		}
	}

	// �������
	if (HasAuthority())
	{
		//CreatePlayerWidget();
		if (ensureAlways(DT_CharacterStats))
			CharacterBaseStat = *DT_CharacterStats->FindRow<FCharacterBaseStat>
			(
				RowName,
				FString::Printf(TEXT("Getting CharacterBaseStat"))
			);

		if (ensure(CharacterBaseStat.Attributes.Num() == FinalStat.Attributes.Num()))
		{
			for (int32 i = 0; i < CharacterBaseStat.Attributes.Num(); ++i)
				FinalStat.Attributes[i] += CharacterBaseStat.Attributes[i];
		}
	}
}

void ADBCharacter::BeginInteract(UDBInteractionComponent* InteractionComponent)
{
}

void ADBCharacter::ExecuteInteract(UDBInteractionComponent* InteractionComponent, ACharacter* OtherCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("this guys interacting with me dude"));

	auto OtherPlayer =Cast<ADBCharacter>(OtherCharacter);
	if(ensureAlways(OtherPlayer) && OtherPlayer->InvMainWidget)
	{
		OtherPlayer->InvMainWidget->InitLootDisplay(this);
		if(OtherPlayer->InvMainWidget->IsLootValid())
		{
			OtherPlayer->InvMainWidget->DisplayInventory(true);
		}
	}
}

void ADBCharacter::InterruptInteract()
{
}

void ADBCharacter::BeginTrace()
{
}

void ADBCharacter::EndTrace()
{
}

bool ADBCharacter::CanInteract() const
{
	return bCanInteract;
}

void ADBCharacter::SetCanInteract(bool bAllowInteract)
{
	bCanInteract = bAllowInteract;
}

void ADBCharacter::OnRep_bCanInteract()
{
	if (bCanInteract) {
		GetMesh()->SetCollisionProfileName("Item");
	}
}

FDisplayInfo ADBCharacter::GetDisplayInfo() const
{
	return FDisplayInfo(TEXT("Loot"), TEXT("PlaceholderName"));
}

void ADBCharacter::SetPlayerName(FString _PlayerName)
{
	PlayerName = _PlayerName;
}

// Called every frame
void ADBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ADBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_DB_Move, ETriggerEvent::Triggered, this, &ADBCharacter::EnhancedMove);
		enhancedInputComponent->BindAction(ia_DB_Jump, ETriggerEvent::Started, this, &ADBCharacter::EnhancedJump);
		enhancedInputComponent->BindAction(ia_DB_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		enhancedInputComponent->BindAction(ia_DB_Look, ETriggerEvent::Triggered, this, &ADBCharacter::EnhancedLook);

		enhancedInputComponent->BindAction(ia_Interact, ETriggerEvent::Started, this, &ADBCharacter::EnhancedInteract);
	}
}

void ADBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBCharacter, MaxHP);
	DOREPLIFETIME(ADBCharacter, CurrHP);
	DOREPLIFETIME(ADBCharacter, bCanInteract);
}



void ADBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//���� �÷��̾��� ���� ����
	CreatePlayerWidget();
}

void ADBCharacter::EnhancedMove(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();
	FVector originVec = FVector(dir.Y, dir.X, 0);
	FVector newVec = GetTransform().TransformVector(originVec);

	AddMovementInput(newVec);
}

void ADBCharacter::EnhancedJump(const struct FInputActionValue& value)
{
	//Jump();
	ServerRPC_DoubleJump();

}

void ADBCharacter::EnhancedStopJump(const struct FInputActionValue& value)
{
	StopJumping();

}

void ADBCharacter::EnhancedLook(const struct FInputActionValue& value)
{
	FVector2D dir = value.Get<FVector2D>();

	AddControllerYawInput(dir.X);
	AddControllerPitchInput(dir.Y);
}

void ADBCharacter::ServerRPC_DoubleJump_Implementation()
{

	MultiRPC_DoubleJump();
}

void ADBCharacter::MultiRPC_DoubleJump_Implementation()
{
	Jump();
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(GetMesh()->GetAnimInstance());
	// ���߿��� ���� ���ߴٸ�
	if (RogueAnim->isFalling && !RogueAnim->isDoubleJumping)
	{
		RogueAnim->AnimNotify_DoubleJumpStart();
		// ���� : �������� �� �������� true�� ���̸� fall loop�� �Ѿ�� �ʴ´�
	}
	// �ٴ��̰� ���� ������
	else if (!RogueAnim->isFalling && RogueAnim->isDoubleJumping)
	{
		RogueAnim->AnimNotify_DoubleJumpEnd();
	}
}

void ADBCharacter::EnhancedInteract(const FInputActionValue& value)
{
	if (ensureAlways(InteractionComp))
		InteractionComp->OnInteract();
}

void ADBCharacter::CreatePlayerWidget()
{
	// ������ �ƴϰų� �÷��̾� ������ ���ٸ� ����
	if (!IsLocallyControlled() || PlayerWidget != nullptr) return;

	// ���� Ŭ���� ��� ����
	PlayerWidget = Cast<UDBPlayerWidget>(CreateWidget(GetWorld(), PlayerWidgetClass));
	PlayerWidget->AddToViewport();
}

void ADBCharacter::OnRep_CurrHP()
{
	// �÷��̾� ������ ������ ����
	if (PlayerWidget == nullptr) return;
	PlayerWidget->UpdateHeathBar(CurrHP, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Testing:%f"), CurrHP);
}

const FFinalStat& ADBCharacter::GetFinalStat() const
{
	return FinalStat;
}


