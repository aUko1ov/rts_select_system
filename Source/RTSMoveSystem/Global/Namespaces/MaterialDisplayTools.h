
#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RTSMoveSystem/Interfaces/ActorSceneRender.h"

namespace PictureDisplayTools
{
	void ApplyMaterialToImage(UImage* Image, UMaterialInterface* MaterialInterface,
		UTextureRenderTarget2D* RenderTarget);

	void ConfigureSceneCaptureForPawn(USceneCaptureComponent2D* SceneCapture,
		const IActorSceneRender* ActorSceneRender, const UTextureRenderTarget2D* RenderTarget);
	
}

