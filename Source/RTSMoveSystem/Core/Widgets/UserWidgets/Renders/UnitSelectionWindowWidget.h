
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnitSelectionWindowWidget.generated.h"


class UShowActorInWidget;

UCLASS()
class RTSMOVESYSTEM_API UUnitSelectionWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetActorRenderOnWidget(AActor* Actor);

private:
	UPROPERTY(meta = (BindWidget))
	UShowActorInWidget* ShowActorInWidget;
	
};
