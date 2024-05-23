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
	virtual void Initialize(UItemObject* ItemObject);
	UItemObject* GetItemObject() const;

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

protected:
	virtual void BeginInteract(UDBInteractionComponent* InteractionComp) override;
	virtual void ExecuteInteract(UDBInteractionComponent* InteractionComp, ACharacter* Character) override;
	virtual void InterruptInteract() override;

	virtual void BeginTrace() override;
	virtual void EndTrace() override;
	
	virtual bool CanInteract() const override;
	virtual void SetCanInteract(bool bCanInteract) override;

	virtual FDisplayInfo GetDisplayInfo() const override;

public:
	/*
	몬타지 플레이 하고 싶은 케릭터를 매개변수에 넣는다.
	if montage successfully plays, returns true. Returns false otherwise.
	*/
	UFUNCTION(BlueprintCallable)
	virtual bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName);
	bool IsMontagePlaying(ACharacter* PlayerCharacter) const;
	void StopMontage(ACharacter* PlayerCharacter);
	UAnimMontage* GetMontage() const;
	void Pickup(AActor* InteractingActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimMontage;
	
	UPROPERTY(Replicated)
	TObjectPtr<UItemObject> ItemObj;

	UFUNCTION()
	void OnRep_bCanInteract();

private:
	UPROPERTY(ReplicatedUsing = "OnRep_bCanInteract")
	bool bCanInteract;
};
