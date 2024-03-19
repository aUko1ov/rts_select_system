
#include "DisplayToolsInitializer.h"
#include "EngineUtils.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/TextureRenderTarget2D.h"

UTextureRenderTarget2D* DisplayToolsInitializer::InitializeRenderTarget(UObject* Outer)
{
	auto* RenderTarget = NewObject<UTextureRenderTarget2D>(Outer, UTextureRenderTarget2D::StaticClass());
	RenderTarget->InitAutoFormat(1024, 1024);
	RenderTarget->ClearColor = FLinearColor::Transparent;
	return RenderTarget;
}

USceneCaptureComponent2D* DisplayToolsInitializer::InitializeSceneCapture(UObject* Outer, UTextureRenderTarget2D* RenderTarget)
{
	auto* SceneCapture = NewObject<USceneCaptureComponent2D>(Outer, USceneCaptureComponent2D::StaticClass());

	SceneCapture->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		
	SceneCapture->ShowOnlyActors.Empty();

	SceneCapture->TextureTarget = RenderTarget;
	SceneCapture->FOVAngle = 45.0f;

	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = true;

	SceneCapture->RegisterComponentWithWorld(Outer->GetWorld());
	return SceneCapture;
}

void DisplayToolsInitializer::HideSkyAndFogActors(USceneCaptureComponent2D* SceneCapture, const UWorld* World)
{
	for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
		SceneCapture->HiddenActors.Add(*It);
		
	for (TActorIterator<ASkyAtmosphere> It(World); It; ++It)
		SceneCapture->HiddenActors.Add(*It);
}