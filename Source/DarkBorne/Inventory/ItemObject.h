#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Math/IntPoint.h"
#include "UObject/NoExportTypes.h"

#include "ItemObject.generated.h"

UCLASS()
class DARKBORNE_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FIntPoint GetDimentions();  // Ã¶ÀÚ ¼öÁ¤
	UFUNCTION(BlueprintPure)
	class UMaterial* GetIcon();
	UFUNCTION(BlueprintPure)
	class AActor* GetItemClass();

protected:
	UFUNCTION()
	virtual UWorld* GetWorld() const override;
private:
	FIntPoint _dimentions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
	class AActor* ItemClass;
};