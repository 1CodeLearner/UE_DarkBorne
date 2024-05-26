// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBInteractionComponent.generated.h"


UENUM(BlueprintType)
enum class EInteractState : uint8
{
	BEGINTRACE UMETA(DisplayName="BeginTrace"),
	ENDTRACE UMETA(DisplayName="EndTrace"),
	BEGININTERACT UMETA(DisplayName="BeginInteract"),
	EXECUTEINTERACT UMETA(DisplayName="ExecuteInteract"),
	INTERRUPTINTERACT UMETA(DisplayName="InterruptInteract")
};

UENUM()
enum class EInteractType : uint8 
{
	BeginInteract, 
	InterruptInteract, 
	ExecuteInteract
};

DECLARE_DELEGATE_TwoParams(FInteractActorUpdateDelegate, AActor* /*InteractingCharacter*/, EInteractState /*InteractState*/);

DECLARE_DELEGATE_TwoParams(FInteractTimeUpdateDelegate, float /*CurrentTime*/, float /*MaxTime*/);

class ACharacter;
class ADBCharacter;
class UDBRogueAttackComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKBORNE_API UDBInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class ADBCharacter;

public:
	UDBInteractionComponent();

	FInteractActorUpdateDelegate OnInteractActorUpdate;
	FInteractTimeUpdateDelegate OnInteractTimeUpdate;
	void DeclareFailedInteraction();
	void StopInteraction();
	bool IsInteracting() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractDistance;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float InteractRadius;
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float interactSpeed;
	
private:
	void OnInteract();
	UFUNCTION(Server, Reliable)
	void Server_TaxiToServer(AActor* Taxied, EInteractType InteractType);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BeginInteract(AActor* Taxied);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InterruptInteract(AActor* Taxied);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExecuteInteract(AActor* Taxied);

	void ExecuteInteraction();
	bool IsDead();

	UPROPERTY()
	TObjectPtr<ACharacter> Character;
	UPROPERTY()
	ADBCharacter* DBCharacter;
	UPROPERTY()
	TObjectPtr<AActor> OverlappingActor;
	UPROPERTY()
	TObjectPtr<UDBRogueAttackComponent> RogueAttackComp;


	bool bInteracting;
	bool bIsUpdating;
	
	//Happens in tick
	bool CanTrace(bool bDebugDraw);
	void UpdateOverlappingActor(bool bDebugDraw);
	void UpdateTimer(float DeltaTime, bool bDebugDraw);

	void SetCanInteract(AActor* InteractingActor, bool bCanInteract);
	UFUNCTION(Server, Reliable)
	void Server_SetCanInteract(AActor* InteractingActor, bool bCanInteract);
	
	float currTime;
	void ResetTimer();
};
