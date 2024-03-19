
#include "MaterialDisplayTools.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

void PictureDisplayTools::ApplyMaterialToImage(UImage* Image, UMaterialInterface* MaterialInterface, UTextureRenderTarget2D* RenderTarget)
{
	if (!Image || !MaterialInterface || !RenderTarget) return;

	if (UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInterface, nullptr))
	{
		DynamicMaterialInstance->SetTextureParameterValue(FName("RenderTargetTexture"), RenderTarget);
		Image->SetBrushFromMaterial(DynamicMaterialInstance);
	}
}

void PictureDisplayTools::ConfigureSceneCaptureForPawn(USceneCaptureComponent2D* SceneCapture,
	const IActorSceneRender* ActorSceneRender, const UTextureRenderTarget2D* RenderTarget)
{
	if (!ActorSceneRender || !SceneCapture || !RenderTarget) return;

	SceneCapture->ShowOnlyActors.Empty();

	SceneCapture->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SceneCapture->AttachToComponent(ActorSceneRender->GetSceneRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}
