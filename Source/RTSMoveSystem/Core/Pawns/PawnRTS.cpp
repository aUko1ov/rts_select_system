
#include "PawnRTS.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RTSMoveSystem/Core/Controllers/SPlayerController.h"
#include "RTSMoveSystem/Core/Widgets/UserWidgets/RTS/RectangleSelectionRTSWidget.h"


APawnRTS::APawnRTS()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

void APawnRTS::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (FMath::Abs(MovementVector.X) > 0.0f)
		{
			const float TurnAngle = MovementVector.X * RotationSpeed * GetWorld()->DeltaTimeSeconds;
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw += TurnAngle;
			SetActorRotation(NewRotation);
		}

		if (FMath::Abs(MovementVector.Y) > 0.0f)
		{
			const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(GetActorRotation());
			FVector Movement = ForwardDirection * MovementVector.Y;
			Movement.Z = 0;
			const FVector NewLocation = GetActorLocation() + (Movement * GetWorld()->DeltaTimeSeconds * MovementSpeed);
			SetActorLocation(NewLocation);
		}
	}
}


void APawnRTS::RemoveInputMappingContext()
{
	SetMouseCursorRTS(false);
	SwitchStateSelecting(false);

	if (RectangleSelectionRTSWidget)
	{
		RectangleSelectionRTSWidget->RemoveFromParent();
		RectangleSelectionRTSWidget->MarkAsGarbage();
		RectangleSelectionRTSWidget = nullptr;
	}
}

void APawnRTS::AddInputMappingContext()
{
	SetMouseCursorRTS(true);

	APlayerController* PlayerController = GetController<APlayerController>();
	RectangleSelectionRTSWidget = CreateWidget<URectangleSelectionRTSWidget>(PlayerController, RectangleSelectionRTSWidgetClass);
	if (RectangleSelectionRTSWidget)
	{
		RectangleSelectionRTSWidget->AddToViewport();
	}
}


bool APawnRTS::GetMousePosition(double& X, double& Y)
{
	if (const APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->GetMousePosition(X, Y);
		return true;
	}
	return false;
}

void APawnRTS::SetMouseCursorRTS(bool CursorVisible)
{
	if (ASPlayerController* PlayerController = GetController<ASPlayerController>())
	{
		PlayerController->SetMouseCursorRTS(CursorVisible);

		if (CursorVisible)
			PlayerController->OnLeftMouseClick.AddDynamic(this, &APawnRTS::HandleLeftMouseClickOnScreen);
		else
			PlayerController->OnLeftMouseClick.RemoveDynamic(this, &APawnRTS::HandleLeftMouseClickOnScreen);
	}
}

void APawnRTS::HandleLeftMouseClickOnScreen(bool bClicked)
{
	bClicked ? BeginSelection() : EndSelection();

	if (bIsSelecting)
	{
		FVector2D CurrentMousePosition;
		GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
		RectangleSelectionRTSWidget->UpdateSelectionTransform(MouseStartPosition, CurrentMousePosition);
	}
}

void APawnRTS::SwitchStateSelecting(bool SwitchSelecting)
{
	bIsSelecting = SwitchSelecting;
	RectangleSelectionRTSWidget->bSelectionActive = SwitchSelecting;
}

void APawnRTS::BeginSelection()
{
	if (!bIsSelecting && GetMousePosition(MouseStartPosition.X, MouseStartPosition.Y))
	{
		SwitchStateSelecting(true);
		RemoveHighlightSelectedUnits();
		SelectedUnits.Empty();
	}
}

void APawnRTS::EndSelection()
{
	if (!bIsSelecting) return;
	SwitchStateSelecting(false);

	FVector2D EndPosition;
	if (GetMousePosition(EndPosition.X, EndPosition.Y) && SelectedUnits.IsEmpty())
	{
		SelectCharactersInRectangle(MouseStartPosition, EndPosition);

		MakeHighlightSelectedUnits();
	}
}

void APawnRTS::MakeHighlightSelectedUnits()
{
	for (IUnit* Unit : SelectedUnits)
	{
		Unit->StartInteractUnitRTS();
	}
}

void APawnRTS::RemoveHighlightSelectedUnits()
{
	for (IUnit* Unit : SelectedUnits)
	{
		Unit->EndInteractUnitRTS();
	}
}


void APawnRTS::CorrectScreenPointsForAspectRatio(FVector2D& ScreenTopLeft, FVector2D& ScreenTopRight, FVector2D& ScreenBottomLeft, FVector2D& ScreenBottomRight, const FVector2D& RectangleSize, float AspectRatio)
{
	if (RectangleSize.X > RectangleSize.Y)
	{
		float CorrectedHeight = RectangleSize.X / AspectRatio;
		float HeightAdjustment = (CorrectedHeight - RectangleSize.Y) / 2;
		ScreenTopLeft.Y -= HeightAdjustment;
		ScreenTopRight.Y -= HeightAdjustment;
		ScreenBottomLeft.Y += HeightAdjustment;
		ScreenBottomRight.Y += HeightAdjustment;
	}
	else
	{
		float CorrectedWidth = RectangleSize.Y * AspectRatio;
		float WidthAdjustment = (CorrectedWidth - RectangleSize.X) / 2;
		ScreenTopLeft.X -= WidthAdjustment;
		ScreenTopRight.X += WidthAdjustment;
		ScreenBottomLeft.X -= WidthAdjustment;
		ScreenBottomRight.X += WidthAdjustment;
	}
}

void APawnRTS::DeprojectScreenPositionToWorld(const FVector2D& ScreenPosition, FVector& OutWorldPosition, FVector& OutWorldDirection)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (!PlayerController) return;

	PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, OutWorldPosition, OutWorldDirection);
	OutWorldPosition += OutWorldDirection * 1000;
}

void APawnRTS::SimpleDrawDebugLine(const FVector& Start, const FVector& End, FColor Color) const
{
	if (bShowDebugLines)
	{
		DrawDebugLine(GetWorld(), Start, End, Color, false, 5.0f);
	}
}

void APawnRTS::HighDensityGridSelection(const FVector& TopLeft, const FVector& TopRight,
	const FVector& BottomLeft, const FVector& BottomRight, const FVector& CameraLocation,
	int32 NumRaysX, int32 NumRaysY)
{
	for (int32 i = 0; i <= NumRaysX; ++i)
	{
		for (int32 j = 0; j <= NumRaysY; ++j)
		{
			float FractionX = static_cast<float>(i) / NumRaysX;
			float FractionY = static_cast<float>(j) / NumRaysY;

			FVector StartPos = FMath::Lerp(TopLeft, BottomLeft, FractionY);
			FVector EndPos = FMath::Lerp(TopRight, BottomRight, FractionY);
			
			FVector HighDensityStart = FMath::Lerp(StartPos, EndPos, FractionX);
			FVector HighDensityEnd = CameraLocation + (HighDensityStart - CameraLocation).GetSafeNormal() * 10000;

			FHitResult HighDensityHitResult;
			GetWorld()->LineTraceSingleByChannel(HighDensityHitResult, CameraLocation, HighDensityEnd, ECC_Pawn);
			ProcessDetectionLineTraceRTS(HighDensityHitResult, CameraLocation, HighDensityEnd);
		}
	}
}

void APawnRTS::ProcessDetectionLineTraceRTS(const FHitResult& HighDensityHitResult,
	const FVector& CameraLocation, const FVector& HighDensityEnd)
{
	if (HighDensityHitResult.bBlockingHit)
	{
		if (IUnit* HitUnit = Cast<IUnit>(HighDensityHitResult.GetActor()))
		{
			SimpleDrawDebugLine(CameraLocation, HighDensityHitResult.ImpactPoint, FColor::Blue);

			if (!SelectedUnits.Contains(HitUnit))
			{
				SelectedUnits.Add(HitUnit);
			}
		}
		else
		{
			SimpleDrawDebugLine(CameraLocation, HighDensityHitResult.ImpactPoint, FColor::Red);
		}
	}
	else
	{
		SimpleDrawDebugLine(CameraLocation, HighDensityEnd, FColor::Red);
	}
}


void APawnRTS::TraceAndDrawCorners(const TArray<FVector>& Corners, const FVector& CameraLocation)
{
	TArray<FVector> HitPoints;
	HitPoints.SetNum(Corners.Num());

	for (int32 i = 0; i < Corners.Num(); ++i)
	{
		FVector StartTrace = CameraLocation;
		FVector EndTrace = StartTrace + (Corners[i] - StartTrace).GetSafeNormal() * 10000;

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility);

		HitPoints[i] = HitResult.bBlockingHit ? HitResult.ImpactPoint : EndTrace;

		SimpleDrawDebugLine(StartTrace, HitPoints[i], FColor::Green);
	}

	for (int32 i = 0; i < HitPoints.Num(); ++i)
	{
		FVector StartPoint = HitPoints[i];
		FVector EndPoint = HitPoints[(i + 1) % HitPoints.Num()];
		SimpleDrawDebugLine(StartPoint, EndPoint, FColor::Yellow);
	}
}

void APawnRTS::SelectCharactersInRectangle(const FVector2D& StartScreenPosition, const FVector2D& EndScreenPosition)
{
	FVector2D RectangleSize = FVector2D(
		FMath::Max(EndScreenPosition.X - StartScreenPosition.X, 1.0f),
		FMath::Max(EndScreenPosition.Y - StartScreenPosition.Y, 1.0f)
	);

	float AspectRatio = FMath::Abs(RectangleSize.X / RectangleSize.Y);

	FVector2D ScreenTopLeft = StartScreenPosition, ScreenBottomRight = EndScreenPosition;
	FVector2D ScreenTopRight(ScreenBottomRight.X, ScreenTopLeft.Y), ScreenBottomLeft(ScreenTopLeft.X, ScreenBottomRight.Y);

	CorrectScreenPointsForAspectRatio(ScreenTopLeft, ScreenTopRight, ScreenBottomLeft, ScreenBottomRight, RectangleSize, AspectRatio);

	FVector WorldTopLeft, WorldDirectionTopLeft, WorldTopRight, WorldDirectionTopRight, WorldBottomLeft, WorldDirectionBottomLeft, WorldBottomRight, WorldDirectionBottomRight;
	DeprojectScreenPositionToWorld(ScreenTopLeft, WorldTopLeft, WorldDirectionTopLeft);
	DeprojectScreenPositionToWorld(ScreenTopRight, WorldTopRight, WorldDirectionTopRight);
	DeprojectScreenPositionToWorld(ScreenBottomLeft, WorldBottomLeft, WorldDirectionBottomLeft);
	DeprojectScreenPositionToWorld(ScreenBottomRight, WorldBottomRight, WorldDirectionBottomRight);

	FVector TopLeft = WorldTopLeft + WorldDirectionTopLeft * 1000;
	FVector BottomRight = WorldBottomRight + WorldDirectionBottomRight * 1000;
	FVector TopRight = WorldTopRight + WorldDirectionTopRight * 1000;
	FVector BottomLeft = WorldBottomLeft + WorldDirectionBottomLeft * 1000;

	FVector CameraLocation;
	FRotator CameraRotation;
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	TArray<FVector> Corners = { TopLeft, TopRight, BottomRight, BottomLeft };

	TraceAndDrawCorners(Corners, CameraLocation);

	int32 NumRaysX = FMath::Max(FMath::CeilToInt((TopRight - TopLeft).Size() / DesiredSpacing), 1);
	int32 NumRaysY = FMath::Max(FMath::CeilToInt((BottomLeft - TopLeft).Size() / DesiredSpacing), 1);

	HighDensityGridSelection(TopLeft, TopRight, BottomLeft, BottomRight, CameraLocation, NumRaysX, NumRaysY);
}
