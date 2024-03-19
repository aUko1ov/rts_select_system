
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RectangleSelectionRTSWidget.generated.h"


UCLASS()
class RTSMOVESYSTEM_API URectangleSelectionRTSWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateSelectionTransform(const FVector2D& SelectionOrigin, const FVector2D& SelectionCurrent);

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const override;

	bool bSelectionActive = false;
protected:
	UPROPERTY(EditAnywhere)
	FSlateBrush Brush;

	UPROPERTY(EditAnywhere)
	FLinearColor Tint;

	FVector2D SelectionPosition;
	FVector2D SelectionSize;
	
};
