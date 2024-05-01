#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Math/IntPoint.h"
#include "UObject/NoExportTypes.h"
#include "../ItemTypes/ItemType.h"
#include "ItemObject.generated.h"

UCLASS(BlueprintAble)
class DARKBORNE_API UItemObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialize(FItem item);

	UFUNCTION(BlueprintPure)
	FIntPoint GetDimentions();  // Ã¶ÀÚ ¼öÁ¤
	UFUNCTION(BlueprintPure)
	class UMaterialInterface* GetIcon();
	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetItemClass();
	UFUNCTION(BlueprintCallable)
	ESlotType GetSlotType() const;

	const FItem& GetItem() const;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnItem(AActor* Initiator, float forwardOffset = 200.f, bool bSetOwner = false);

protected:
	UFUNCTION()
	virtual UWorld* GetWorld() const override;

	/** return the stat id to use for this tickable **/
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<AActor> ItemClass;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

private:
	UPROPERTY(Replicated)
	FItem Item = FItem();

	FIntPoint _dimentions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
};