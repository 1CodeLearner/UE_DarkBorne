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
	FIntPoint GetDimensions();  // Ã¶ÀÚ ¼öÁ¤
	UFUNCTION(BlueprintPure)
	class UMaterial* GetIcon();
	
private:
	FIntPoint _dimensions;  // Ã¶ÀÚ ¼öÁ¤
	class UMaterial* Icon;
};