
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShowActorInWidget.generated.h"


class UImage; 
class IActorSceneRender;

UCLASS()
class RTSMOVESYSTEM_API UShowActorInWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSceneCapturePosition(float DistanceFrom) const;
	void SetActorRenderOnWidget(AActor* ActorRenderIn);
	void InitToolsRenderCapture();
	void SetRenderTargetTexture() const;

	bool bIsInWidgetComponent = false;
	float BaseDistance = 300.f;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> MaterialRenderTarget;
	
	UPROPERTY(Transient)
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;

	UPROPERTY(Transient)
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;

	UPROPERTY(Transient)
	FVector2D PreviousViewportSize = FVector2D(-1, -1);

	TScriptInterface<IActorSceneRender> ActorSceneRender;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void HandleViewportSizeChange() const;
	void UpdateRenderPosition(const FVector2D& CurrentSize) const;
	void HandleViewportSizeChange(const FVector2D& NewSize) const;
	void UpdateSceneCapturePosition(const FVector2D& ViewportSize) const;
	void ResizeRenderTarget(const FVector2D& ViewportSize) const;
	
};
