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

	void BeginInteract_Implementation(ACharacter* Character);
	void ExecuteInteract_Implementation(ACharacter* Character);
	void EndInteract_Implementation();

	/*
	��Ÿ�� �÷��� �ϰ� ���� �ɸ��͸� �Ű������� �ִ´�.
	if montage successfully plays, returns true. Returns false otherwise.
	*/
	UFUNCTION(BlueprintCallable)
	bool PlayMontage(ACharacter* PlayerCharacter, FName SectionName);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UStaticMeshComponent> SMComp;

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TObjectPtr<USceneComponent> SceneComp;

	//�ݸ��� ĸ�� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComp;

	void Pickup(AActor* InteractingActor);

	//
	void Initialize(UItemObject* ItemObject);

	UItemObject* GetItemObject() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimMontage;

private:
	UPROPERTY()
	TObjectPtr<UItemObject> ItemObj;
	FName Id;
	
};
