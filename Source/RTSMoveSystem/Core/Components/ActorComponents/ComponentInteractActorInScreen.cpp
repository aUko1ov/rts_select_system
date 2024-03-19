
#include "ComponentInteractActorInScreen.h"

#include "Blueprint/UserWidget.h"
#include "RTSMoveSystem/Core/Controllers/SPlayerController.h"
#include "RTSMoveSystem/Core/Widgets/UserWidgets/Renders/UnitSelectionWindowWidget.h"


UComponentInteractActorInScreen::UComponentInteractActorInScreen()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UComponentInteractActorInScreen::ShowActorOnWidget(AActor* ActorRenderIn)
{
	if(UnitSelectionWindowWidgetClass == nullptr || !ActorRenderIn) return;

	SPlayerController = Cast<ASPlayerController>(GetWorld()->GetFirstPlayerController());
	if(SPlayerController && SPlayerController->GetHUD() && !ActiveUnitSelectionWindowWidget)
	{
		ActiveUnitSelectionWindowWidget = CreateWidget<UUnitSelectionWindowWidget>(SPlayerController, UnitSelectionWindowWidgetClass);
		if(ActiveUnitSelectionWindowWidget)
		{
			ActiveUnitSelectionWindowWidget->SetActorRenderOnWidget(ActorRenderIn);
			//GetMarketUnitsWidgetsFromDataTable();
			ActiveUnitSelectionWindowWidget->AddToViewport();

		}
	}
}

void UComponentInteractActorInScreen::CloseActiveWidget()
{
	if(ActiveUnitSelectionWindowWidget)
	{
		ActiveUnitSelectionWindowWidget->RemoveFromParent();
		ActiveUnitSelectionWindowWidget = nullptr;
	}
}