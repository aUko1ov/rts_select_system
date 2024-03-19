
#include "RectangleSelectionRTSWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"


void URectangleSelectionRTSWidget::UpdateSelectionTransform(const FVector2D& SelectionOrigin, const FVector2D& SelectionCurrent)
{
	const float MaxX = FMath::Max(SelectionOrigin.X, SelectionCurrent.X);
	const float MaxY = FMath::Max(SelectionOrigin.Y, SelectionCurrent.Y);
	const float MinX = FMath::Min(SelectionOrigin.X, SelectionCurrent.X);
	const float MinY = FMath::Min(SelectionOrigin.Y, SelectionCurrent.Y);

	const FVector2D SelectionBoxMin = FVector2D(MinX, MinY);
	const FVector2D SelectionBoxMax = FVector2D(MaxX, MaxY);

	const FGeometry ViewportGeometry =  UWidgetLayoutLibrary::GetViewportWidgetGeometry(this);
	const FVector2D ScreenRatio = FVector2D(ViewportGeometry.GetLocalSize() / ViewportGeometry.GetAbsoluteSize());
	
	SelectionPosition = SelectionBoxMin * ScreenRatio;
	SelectionSize = (SelectionBoxMax - SelectionBoxMin) * ScreenRatio;
}

int32 URectangleSelectionRTSWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (bSelectionActive)
	{
		const FSlateLayoutTransform LayoutTransform(1.0f, SelectionPosition);
		const FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry(SelectionSize, LayoutTransform);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			MaxLayerId + 1,
			PaintGeometry,
			&Brush,
			ESlateDrawEffect::None,
			Tint
		);

		MaxLayerId++;
	}

	return MaxLayerId;
}
