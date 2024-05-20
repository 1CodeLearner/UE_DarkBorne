#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Math/IntPoint.h"
#include "UObject/NoExportTypes.h"
#include "../ItemTypes/ItemType.h"
#include "ItemObject.generated.h"

class ADBItem;
class UMaterialInterface;

USTRUCT()
struct FItemData
{
	GENERATED_BODY()

	FItemData()
	{
		ItemActor = nullptr;
	}

	FItemData(FItem _Item, ADBItem* _ItemActor)
	{
		Item = _Item;
		ItemActor = _ItemActor;
	}

	UPROPERTY()
	FItem Item = FItem();

	UPROPERTY()
	TObjectPtr<ADBItem> ItemActor;
};

UCLASS(BlueprintAble)
class DARKBORNE_API UItemObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialize(FItem _Item, ADBItem* _ItemActor = nullptr);

	UFUNCTION(BlueprintCallable)
	ADBItem* SpawnItem(AActor* Initiator, bool bSetOwner, FTransform Trans, float forwardOffset);
	UFUNCTION(BlueprintCallable)
	bool ImplementsItemInterface() const;

	UFUNCTION(BlueprintCallable)
	void SetItemActor(ADBItem* _ItemActor);
	UFUNCTION(BlueprintCallable)
	bool HasItemActor() const;
	ADBItem* GetItemActor() const;
	void TryDestroyItemActor();

	UFUNCTION(BlueprintCallable)
	FText GetDisplayName() const;
	UFUNCTION(BlueprintCallable)
	FText GetLoreText() const;
	UFUNCTION(BlueprintCallable)
	EItemType GetItemType() const;

	FName GetId() const;
	
	UFUNCTION(BlueprintCallable)
	FText GetBaseStatsText() const;
	UFUNCTION(BlueprintCallable)
	TArray<FText> GetEnchantmentsTexts() const;
	UFUNCTION(BlueprintCallable)
	FText GetEffectText() const;
	UFUNCTION(BlueprintCallable)
	FText GetCategoryText() const;




	UFUNCTION(BlueprintCallable)
	float GetRarityValue() const;
	UFUNCTION(BlueprintCallable)
	FRarity GetRarity() const;
	UFUNCTION(BlueprintCallable)
	FDarkBorneStats GetEnchantments() const;
	
	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimentions() const;  // Ã¶ÀÚ ¼öÁ¤
	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetIcon() const;
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIconTexture() const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetItemClass() const;
	UFUNCTION(BlueprintCallable)
	ESlotType GetSlotType() const;

	const FItem& GetItem() const;

	/*UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SpawnItem(AActor* Initiator, float forwardOffset = 200.f, bool bSetOwner = false);*/

protected:
	UFUNCTION()
	virtual UWorld* GetWorld() const override;

	/** return the stat id to use for this tickable **/
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<AActor> ItemClass;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

private:
	UPROPERTY(Replicated)
	FItemData ItemData;

	FIntPoint _dimentions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
};