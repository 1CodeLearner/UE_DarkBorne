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
#include "../Inventory/LootInventoryComponent.h"
#include "../Inventory/LootEquipmentComponent.h"
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

	LootInventoryComponent = CreateDefaultSubobject<ULootInventoryComponent>("LootInventoryComp");
	LootEquipmentComponent = CreateDefaultSubobject<ULootEquipmentComponent>("LootEquipmentComp");

	InteractDistance = 400.f;
	InteractionComp = CreateDefaultSubobject<UDBInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ProjectileSpawnPos->GetComponentLocation()
	AActor* actor = GetOwner();
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
			subSystem->AddMappingContext(imc_DBMapping, 0);
		}
		//클라들은 여기서 모두 위젯을 생성한다
		if (!HasAuthority())
		{
			CreatePlayerWidget();
		}
	}

	// 서버라면
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
}



void ADBCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버 플레이어의 위젯 생성
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
	// 공중에서 덮점 안했다면
	if (RogueAnim->isFalling && !RogueAnim->isDoubleJumping)
	{
		RogueAnim->AnimNotify_DoubleJumpStart();
		// 문제 : 공중제비를 다 돌기전에 true를 맥이면 fall loop로 넘어가지 않는다
	}
	// 바닥이고 덮점 했으면
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
	// 내것이 아니거나 플레이어 위젯이 없다면 리턴
	if (!IsLocallyControlled() || PlayerWidget != nullptr) return;

	// 위젯 클래스 담고 생성
	PlayerWidget = Cast<UDBPlayerWidget>(CreateWidget(GetWorld(), PlayerWidgetClass));
	PlayerWidget->AddToViewport();
}

void ADBCharacter::OnRep_CurrHP()
{
	// 플레이어 위젯이 없으면 리턴
	if (PlayerWidget == nullptr) return;
	PlayerWidget->UpdateHeathBar(CurrHP, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Testing:%f"), CurrHP);
}

const FFinalStat& ADBCharacter::GetFinalStat() const
{
	return FinalStat;
}


