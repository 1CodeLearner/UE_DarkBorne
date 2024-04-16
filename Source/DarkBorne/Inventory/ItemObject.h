#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Math/IntPoint.h"
#include "UObject/NoExportTypes.h"

#include "ItemObject.generated.h"

UCLASS(BlueprintAble)
class DARKBORNE_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FIntPoint GetDimentions();  // Ã¶ÀÚ ¼öÁ¤
	UFUNCTION(BlueprintPure)
	class UMaterial* GetIcon();
	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetItemClass();

protected:
	UFUNCTION()
	virtual UWorld* GetWorld() const override;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	TSubclassOf<AActor> ItemClass;
private:
	FIntPoint _dimentions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
};