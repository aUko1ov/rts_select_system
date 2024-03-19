
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentInteractActorInScreen.generated.h"


class UUnitSelectionWindowWidget;
class ASPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSMOVESYSTEM_API UComponentInteractActorInScreen : public UActorComponent
{
	GENERATED_BODY()

public:	
	UComponentInteractActorInScreen();
	void ShowActorOnWidget(AActor* ActorRenderIn);
	void CloseActiveWidget();

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUnitSelectionWindowWidget> UnitSelectionWindowWidgetClass;

	UPROPERTY()
	ASPlayerController* SPlayerController;
	
private:
	UPROPERTY(Transient)
	UUnitSelectionWindowWidget* ActiveUnitSelectionWindowWidget;
	
};
