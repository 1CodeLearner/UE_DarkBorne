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
#include "../Status/CharacterStatusComponent.h"
#include "../Status/DBEffectComponent.h"

// Sets default values
ADBCharacter::ADBCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	EquipmentComponent = CreateDefaultSubobject<UDBEquipmentComponent>("EquipmentComp");
	PlayerEquipmentComp = CreateDefaultSubobject<UPlayerEquipmentComponent>("PlayerEquipmentComp");
	// 플레이어 메쉬 프로필
	GetMesh()->SetCollisionProfileName(TEXT("PlayerMeshColl"));
	GetMesh()->SetGenerateOverlapEvents(true);
	// 플레이어 캡슐 프로필
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerColl"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	// 플레이어 상태 컴포넌트
	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>("CharacterStatusComp");
	// 인터렉션 범위
	InteractDistance = 400.f;
	InteractionComp = CreateDefaultSubobject<UDBInteractionComponent>("InteractionComp");
	// 버프 컴포넌트
	EffectComp = CreateDefaultSubobject<UDBEffectComponent>("EffectComp");
}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetInteractionTime(31.f);

	// 내 것이라면 
	if (IsLocallyControlled())
	{
		//get APlayerController
		APlayerController* playerContoller = Cast<APlayerController>(GetController());
		if (playerContoller == nullptr) return;
		//get subSystem
		UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerContoller->GetLocalPlayer());

		//서브시스템을 가져왔다면
		if (subSystem)
		{
			//imc 맵핑
			subSystem->AddMappingContext(imc_DBMapping, 0);
		}
		//클라들은 여기서 모두 위젯을 생성한다
		if (!HasAuthority())
		{
			CreatePlayerWidget();
		}
	}
}

void ADBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CharacterStatusComponent)
	{
		CharacterStatusComponent->Initialize();

		MaxHP = CharacterStatusComponent->MaxHP;
		CurrHP = MaxHP;
	}
}

void ADBCharacter::BeginInteract(UDBInteractionComponent* InteractionComponent)
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ADBCharacter::ExecuteInteract(UDBInteractionComponent* InteractionComponent, ACharacter* OtherCharacter)
{
	auto OtherPlayer = Cast<ADBCharacter>(OtherCharacter);
	if (ensureAlways(OtherPlayer) && OtherPlayer->InvMainWidget)
	{
		OtherPlayer->InvMainWidget->InitLootDisplay(this);
		if (OtherPlayer->InvMainWidget->IsLootValid())
		{
			OtherPlayer->DisplayInventory(true);
		}
	}

	/*if (HasNetOwner())
		PlayerEquipmentComp->Server_AddItemAt(nullptr, 10);
	else
		UE_LOG(LogTemp, Warning, TEXT("HasNetOwner"));

	if(	HasLocalNetOwner())
		PlayerEquipmentComp->Server_AddItemAt(nullptr, 10);
	else
		UE_LOG(LogTemp, Warning, TEXT("HasLocalNetOwner"));*/
}

void ADBCharacter::InterruptInteract()
{
}

void ADBCharacter::BeginTrace()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void ADBCharacter::EndTrace()
{
	GetMesh()->SetRenderCustomDepth(false);
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

		enhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ADBCharacter::EnhancedInteract);
		enhancedInputComponent->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ADBCharacter::EnhancedInventory);
	}
}

void ADBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBCharacter, CharacterStatusComponent);
	DOREPLIFETIME(ADBCharacter, MaxHP);
	DOREPLIFETIME(ADBCharacter, CurrHP);
	DOREPLIFETIME(ADBCharacter, bCanInteract);
}



void ADBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버 플레이어의 위젯 생성
	if (HasAuthority())
	{
		CreatePlayerWidget();
	}
}

void ADBCharacter::EnhancedMove(const struct FInputActionValue& value)
{	// 방향
	FVector2D dir = value.Get<FVector2D>();
	FVector originVec = FVector(dir.Y, dir.X, 0);
	FVector newVec = GetTransform().TransformVector(originVec);

	if (InteractionComp && InteractionComp->IsInteracting())
		return;
	AddMovementInput(newVec);
}

void ADBCharacter::EnhancedJump(const struct FInputActionValue& value)
{
	ServerRPC_DoubleJump();
}

void ADBCharacter::ServerRPC_DoubleJump_Implementation()
{
	MultiRPC_DoubleJump();
}

void ADBCharacter::MultiRPC_DoubleJump_Implementation()
{
	Jump();
	UDBRogueAnimInstance* RogueAnim = Cast<UDBRogueAnimInstance>(GetMesh()->GetAnimInstance());
	// 공중에서 덮점 안했다면
	if (RogueAnim->isFalling && !RogueAnim->isDoubleJumping)
	{
		RogueAnim->AnimNotify_DoubleJumpStart();
	}
	// 바닥이고 덮점 했으면
	else if (!RogueAnim->isFalling && RogueAnim->isDoubleJumping)
	{
		RogueAnim->AnimNotify_DoubleJumpEnd();
	}
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

void ADBCharacter::EnhancedInteract(const FInputActionValue& value)
{
	if (ensureAlways(InteractionComp))
		InteractionComp->OnInteract();
}

void ADBCharacter::EnhancedInventory(const FInputActionValue& value)
{
	bool input = value.Get<bool>();
	if (ensureAlways(InvMainWidget))
	{
		if (InteractionComp && InteractionComp->IsInteracting())
			return;
		if (InvMainWidget->IsInViewport())
		{
			DisplayInventory(false);
		}
		else
		{
			DisplayInventory(true);
		}
	}
}

void ADBCharacter::DisplayInventory(bool bEnabled)
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (bEnabled)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (EnhancedInputSubSystem)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, InvMainWidget, EMouseLockMode::LockAlways, false);

			PC->bShowMouseCursor = true;

			EnhancedInputSubSystem->AddMappingContext(IMC_Inventory, 0);
			EnhancedInputSubSystem->RemoveMappingContext(imc_DBMapping);
		}
		InvMainWidget->DisplayInventory(true);
	}
	else
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (EnhancedInputSubSystem)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
			PC->bShowMouseCursor = false;

			EnhancedInputSubSystem->AddMappingContext(imc_DBMapping, 0);
			EnhancedInputSubSystem->RemoveMappingContext(IMC_Inventory);
		}
		InvMainWidget->DisplayInventory(false);
	}
}

void ADBCharacter::CreatePlayerWidget()
{
	// 내것이 아니거나 플레이어 위젯이 없다면 리턴
	if (!IsLocallyControlled() || PlayerWidget != nullptr) return;

	// 위젯 클래스 담고 생성
	PlayerWidget = Cast<UDBPlayerWidget>(CreateWidget(GetWorld(), PlayerWidgetClass));
	PlayerWidget->AddToViewport();
}
// 플레이어 위젯의 HP바 갱신
void ADBCharacter::OnRep_CurrHP(float Old)
{
	// 플레이어 위젯이 없으면 리턴

	UE_LOG(LogTemp, Warning, TEXT("Old:%f"), Old);
	UE_LOG(LogTemp, Warning, TEXT("New:%f"), CurrHP);

	if (PlayerWidget == nullptr) return;
	
	PlayerWidget->UpdateHeathBar(CharacterStatusComponent->CurrHP, CharacterStatusComponent->MaxHP);
	if (CurrHP < Old) {
		PlayerWidget->ShowDamageUI();
	}
	UE_LOG(LogTemp, Warning, TEXT("Testing:%f"), CharacterStatusComponent->CurrHP);
}


