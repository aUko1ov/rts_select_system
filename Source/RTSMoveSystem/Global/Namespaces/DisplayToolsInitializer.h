
#pragma once

#include "CoreMinimal.h"


namespace DisplayToolsInitializer
{
	UTextureRenderTarget2D* InitializeRenderTarget(UObject* Outer);

	USceneCaptureComponent2D* InitializeSceneCapture(UObject* Outer, UTextureRenderTarget2D* RenderTarget);

	void HideSkyAndFogActors(USceneCaptureComponent2D* SceneCapture, const UWorld* World);
	
}

