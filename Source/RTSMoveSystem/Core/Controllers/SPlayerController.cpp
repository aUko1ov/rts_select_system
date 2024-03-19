
#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	IUnitInterface.SetObject(InPawn);
	IUnitInterface.SetInterface(Cast<IUnit>(InPawn));

	if (IUnitInterface)
		IUnitInterface->AddInputMappingContext();
}

void ASPlayerController::BeginPlay()
{
	OnPossessedPawnChanged.AddUniqueDynamic(this, &ASPlayerController::UpdatePawnInputContext);

	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASPlayerController::UpdatePawnInputContext(APawn* OldPawnInner, APawn* NewPawnInner)
{
	if (IUnit* UnitOldPawn = Cast<IUnit>(OldPawnInner))
	{
		UnitOldPawn->RemoveInputMappingContext();
	}
}


void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASPlayerController::Jump);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPlayerController::Look);


}

void ASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	HandleLeftMouseClick();
}

void ASPlayerController::HandleLeftMouseClick() const
{
	const bool bLeftMouseClick = IsInputKeyDown(EKeys::LeftMouseButton);

	OnLeftMouseClick.Broadcast(bLeftMouseClick);
}


void ASPlayerController::SetMouseCursorRTS(bool CursorVisible)
{
	bShowMouseCursor = CursorVisible;
	FInputModeGameAndUI InputMode;

	if (CursorVisible)
	{
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
	}
	else
	{
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(true);
	}

	SetInputMode(InputMode);
}
