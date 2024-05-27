// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../ItemTypes/ItemType.h"
#include "../Framework/Interfaces/InteractionInterface.h"
#include "DBCharacter.generated.h"

class UDataTable;

class UInventoryMainWidget;
class ULootDisplayWidget;

class UDBEquipmentComponent;
class UPlayerEquipmentComponent;

class UDBInteractionComponent;

class UDBEffectComponent;

class USoundBase;

UCLASS()
class DARKBORNE_API ADBCharacter : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBCharacter();

	bool IsInventoryInViewport() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void BeginInteract(UDBInteractionComponent* InteractionComponent) override;
	virtual void ExecuteInteract(UDBInteractionComponent* InteractionComponent, ACharacter* OtherCharacter) override;
	virtual void InterruptInteract() override;

	virtual void BeginTrace() override;
	virtual void EndTrace() override;

	virtual bool CanInteract() const override;
	virtual void SetCanInteract(bool bAllowInteract) override;
	UPROPERTY(ReplicatedUsing = "OnRep_bCanInteract")
	bool bCanInteract;
	UFUNCTION()
	void OnRep_bCanInteract();

	virtual FDisplayInfo GetDisplayInfo() const override;
	FString PlayerName;
	void SetPlayerName(FString _PlayerName);
public:
	UFUNCTION(BlueprintCallable)
	void DisplayInventory(bool bEnabled);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;

public:

	//Inventory Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UDBEquipmentComponent* EquipmentComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	UPlayerEquipmentComponent* PlayerEquipmentComp;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Settings")
	class UCharacterStatusComponent* CharacterStatusComponent;

	//Inventory UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UInventoryMainWidget> InvMainWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
	UInventoryMainWidget* InvMainWidget;

	//Interaction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UDBInteractionComponent> InteractionComp;

	//Item Effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UDBEffectComponent> EffectComp;

	//Being Looted Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAudioComponent> AudioComp_Looting;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDBPlayerWidget* PlayerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UDBPlayerWidget> PlayerWidgetClass;
public:
	UPROPERTY(EditAnywhere)
	class UArrowComponent* ThrowKnifePos;
public:
	//Input for Gameplay
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_DBMapping;
	//Input for inventory
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC_Inventory;

	//player input
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Look;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_DB_Jump;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> IA_Interact;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> IA_Inventory;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	float MaxHP;
	// 현재 체력을 계속 업뎃시키는 함수를 replicate 이거는 클라에서만 호출됨
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrHP, EditAnywhere)
	float CurrHP;

public:
	//Default 이동 관련 함수들
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedJump(const struct FInputActionValue& value);
	void EnhancedStopJump(const struct FInputActionValue& value);
	void EnhancedLook(const struct FInputActionValue& value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DoubleJump();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DoubleJump();

	virtual void EnhancedInteract(const struct FInputActionValue& value);
	virtual void EnhancedInventory(const struct FInputActionValue& value);
public:
	void CreatePlayerWidget();

	UFUNCTION()
	void OnRep_CurrHP(float Old);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractDistance;

};
