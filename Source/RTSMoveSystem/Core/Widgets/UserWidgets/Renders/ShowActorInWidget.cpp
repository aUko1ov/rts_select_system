
#include "ShowActorInWidget.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RTSMoveSystem/Global/Namespaces/DisplayToolsInitializer.h"
#include "RTSMoveSystem/Global/Namespaces/MaterialDisplayTools.h"
#include "RTSMoveSystem/Interfaces/ActorSceneRender.h"


void UShowActorInWidget::SetSceneCapturePosition(float DistanceFrom) const
{
	if (!ActorSceneRender || !SceneCapture) return;

	const FTransform CameraSceneTransform = ActorSceneRender->CalculateCameraSceneTransform(DistanceFrom);
	SceneCapture->SetWorldTransform(CameraSceneTransform);
}

void UShowActorInWidget::SetActorRenderOnWidget(AActor* ActorRenderIn)
{
	ActorSceneRender.SetObject(ActorRenderIn);
	ActorSceneRender.SetInterface(Cast<IActorSceneRender>(ActorRenderIn));

	InitToolsRenderCapture();

	PictureDisplayTools::ConfigureSceneCaptureForPawn(SceneCapture, ActorSceneRender.GetInterface(), RenderTarget);
	ActorSceneRender->SetActorToSceneCapture(SceneCapture->ShowOnlyActors, SceneCapture->HiddenActors);
	HandleViewportSizeChange();
}


void UShowActorInWidget::InitToolsRenderCapture()
{
	if (RenderTarget == nullptr)
	{
		RenderTarget = DisplayToolsInitializer::InitializeRenderTarget(this);
	}

	if (SceneCapture == nullptr)
	{
		SceneCapture = DisplayToolsInitializer::InitializeSceneCapture(this, RenderTarget);
		DisplayToolsInitializer::HideSkyAndFogActors(SceneCapture, GetWorld());
	}
}

void UShowActorInWidget::SetRenderTargetTexture() const
{
	PictureDisplayTools::ApplyMaterialToImage(ImageWidget, MaterialRenderTarget, RenderTarget);
}

void UShowActorInWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (const FVector2D CurrentSize = MyGeometry.GetLocalSize(); PreviousViewportSize != CurrentSize)
	{
		HandleViewportSizeChange(CurrentSize);
		PreviousViewportSize = CurrentSize;
	}
}

void UShowActorInWidget::HandleViewportSizeChange() const
{
	if (PreviousViewportSize == FVector2D(-1, -1)) return;
	HandleViewportSizeChange(PreviousViewportSize);
}

void UShowActorInWidget::HandleViewportSizeChange(const FVector2D& NewSize) const
{
	UpdateRenderPosition(NewSize);
}

void UShowActorInWidget::UpdateRenderPosition(const FVector2D& CurrentSize) const
{
	if (!RenderTarget || !SceneCapture || !ActorSceneRender)
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateRenderTargetSizeAndPosition: Missing components."));
		return;
	}

	UpdateSceneCapturePosition(CurrentSize);
	ResizeRenderTarget(CurrentSize);
}

void UShowActorInWidget::UpdateSceneCapturePosition(const FVector2D& ViewportSize) const
{
	const float AspectRatio = ViewportSize.X / ViewportSize.Y;
	const float LengthOfCenter = BaseDistance * AspectRatio;
	UE_LOG(LogTemp, Log, TEXT("AspectRatio: %f, LengthOfCenter: %f"), AspectRatio, LengthOfCenter);
	SetSceneCapturePosition(LengthOfCenter);
}

void UShowActorInWidget::ResizeRenderTarget(const FVector2D& ViewportSize) const
{
	if (RenderTarget->SizeX != ViewportSize.X || RenderTarget->SizeY != ViewportSize.Y)
	{
		if (!bIsInWidgetComponent)
		{
			RenderTarget->ResizeTarget(ViewportSize.X, ViewportSize.Y);
		}
		SetRenderTargetTexture();
	}
}

