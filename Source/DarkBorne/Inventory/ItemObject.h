#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Math/IntPoint.h"
#include "UObject/NoExportTypes.h"
#include "../ItemTypes/ItemType.h"
#include "ItemObject.generated.h"

UCLASS(BlueprintAble)
class DARKBORNE_API UItemObject : public UObject
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

protected:
	UFUNCTION()
	virtual UWorld* GetWorld() const override;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSubclassOf<AActor> ItemClass;
private:
	FItem Item = FItem();

	FIntPoint _dimentions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
};