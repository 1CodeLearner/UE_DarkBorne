// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Framework/Interfaces/InteractionInterface.h"
#include "DBItem.generated.h"

class UStaticMeshComponent;
class UItemObject;
class ACharacter;

UCLASS()
class DARKBORNE_API ADBItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADBItem();
	virtual void Tick(float DeltaTime) override;
	void Initialize(UItemObject* ItemObject);
	
	//콜리전 캡슐 컴포넌트
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<USceneComponent> SceneComp;

	void BeginInteract_Implementation(UDBInteractionComponent* InteractionComp);
	void ExecuteInteract_Implementation(ACharacter* Character);
	void InterruptInteract_Implementation();
	UItemObject* GetItemObject_Implementation() const;
	
	virtual void BeginTrace() override;
	virtual void EndTrace() override;
	virtual FDisplayInfo GetDisplayInfo() const override;
	virtual bool CanInteract() const override;
	virtual void SetCanInteract(bool bCanInteract) override;

public:
	/*
	몬타지 플레이 하고 싶은 케릭터를 매개변수에 넣는다.
	if montage successfully plays, returns true. Returns false otherwise.
	*/
	UFUNCTION(BlueprintCallable)
	bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName);

	void Pickup(AActor* InteractingActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimMontage;

private:
	FName Id;
	
	UPROPERTY(Replicated)
	TObjectPtr<UItemObject> ItemObj;
	UPROPERTY(ReplicatedUsing = "OnRep_bCanInteract")
	bool bCanInteract;

	UFUNCTION()
	void OnRep_bCanInteract();
};
