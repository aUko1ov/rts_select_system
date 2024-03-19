
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "RTSMoveSystem/Interfaces/Unit.h"
#include "PawnRTS.generated.h"

class UCameraComponent;
class USpringArmComponent;
class URectangleSelectionRTSWidget;
class UInputAction;
class UInputMappingContext;

UCLASS()
class RTSMOVESYSTEM_API APawnRTS : public APawn, public IUnit
{
	GENERATED_BODY()

public:
	virtual void Move(const FInputActionValue& Value) override;

	virtual void RemoveInputMappingContext() override;
	virtual void AddInputMappingContext() override;

protected:
	APawnRTS();

#pragma region CursorSelectionRTS
	void SelectCharactersInRectangle(const FVector2D& StartScreenPosition, const FVector2D& EndScreenPosition);

	void BeginSelection();

	void MakeHighlightSelectedUnits();
	void RemoveHighlightSelectedUnits();

	void EndSelection();
	
	bool GetMousePosition(double& X, double& Y);

	void SetMouseCursorRTS(bool CursorVisible);

	void CorrectScreenPointsForAspectRatio(FVector2D& ScreenTopLeft, FVector2D& ScreenTopRight,
		FVector2D& ScreenBottomLeft,
		FVector2D& ScreenBottomRight, const FVector2D& RectangleSize,
		float AspectRatio);

	void DeprojectScreenPositionToWorld(const FVector2D& ScreenPosition, FVector& OutWorldPosition,
		FVector& OutWorldDirection);
	
	void SimpleDrawDebugLine(const FVector& Start, const FVector& End, FColor Color) const;

	UFUNCTION()
	void HandleLeftMouseClickOnScreen(bool bClicked);
	
	void SwitchStateSelecting(bool SwitchSelecting);

	void TraceAndDrawCorners(const TArray<FVector>& Corners, const FVector& CameraLocation);

	void HighDensityGridSelection(const FVector& TopLeft, const FVector& TopRight, const FVector& BottomLeft,
		const FVector& BottomRight, const FVector& CameraLocation, int32 NumRaysX, int32 NumRaysY);
	void ProcessDetectionLineTraceRTS(const FHitResult& HighDensityHitResult, const FVector& CameraLocation, const FVector& HighDensityEnd);
#pragma endregion 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MovementSpeed{ 500.0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RotationSpeed{ 100.0 };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

#pragma region CursorSelectionRTS
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<URectangleSelectionRTSWidget> RectangleSelectionRTSWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<URectangleSelectionRTSWidget> RectangleSelectionRTSWidget;

	UPROPERTY(Transient)
	TArray<IUnit*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bShowDebugLines{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DesiredSpacing{ 5.0f };

	FVector2D MouseStartPosition;
	bool bIsSelecting = false;
#pragma endregion 
};
