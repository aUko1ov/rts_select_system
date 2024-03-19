
#include "SCharacter.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "RTSMoveSystem/Core/Components/ActorComponents/ComponentInteractActorInScreen.h"
#include "RTSMoveSystem/Core/Components/ActorComponents/ProceduralPyramidRTSComponent.h"


ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}


void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASCharacter::Jump()
{
	Super::Jump();
}

FTransform ASCharacter::CalculateCameraSceneTransform(float DistanceFrom) const
{
	const FVector CameraOffset = GetActorForwardVector() * DistanceFrom;
	const FVector NewCameraLocation = GetActorLocation() + CameraOffset;
	const FRotator NewCameraRotation = UKismetMathLibrary::FindLookAtRotation(NewCameraLocation, GetActorLocation());
	return FTransform(NewCameraRotation, NewCameraLocation);
}

USceneComponent* ASCharacter::GetSceneRootComponent() const
{
	return GetRootComponent();
}

void ASCharacter::PrepareSelfForSceneRender()
{
	DisablingCollisions();
	DisablingGameLogic();
	SetSelfVisibleInSceneCaptureOnly();
}

void ASCharacter::DisablingCollisions()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->Deactivate();
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->SetComponentTickEnabled(false);
		GetCharacterMovement()->SetUpdatedComponent(nullptr);
	}
	
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetVisibility(false); 
		GetCapsuleComponent()->SetSimulatePhysics(false);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GetMesh())
	{
		GetMesh()->SetEnableGravity(false);
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASCharacter::DisablingGameLogic()
{
	SetActorTickEnabled(false);

	AIControllerClass = nullptr;
	
	if (AAIController* PawnInnerAI = GetController<AAIController>())
		PawnInnerAI->UnPossess();

	if (UAIPerceptionStimuliSourceComponent* StimuliSourceComponent = FindComponentByClass<UAIPerceptionStimuliSourceComponent>())
		StimuliSourceComponent->UnregisterFromPerceptionSystem();
}

void ASCharacter::SetSelfVisibleInSceneCaptureOnly()
{
	TArray<AActor*> OutActors;
	GetAttachedActors(OutActors);
	OutActors.Add(this);

	for (const AActor* Actor : OutActors)
	{
		Actor->ForEachComponent<UPrimitiveComponent>(true, [](UPrimitiveComponent* PrimitiveComponent)
			{
				if (PrimitiveComponent)
					PrimitiveComponent->SetVisibleInSceneCaptureOnly(true);
			});
	}
}

void ASCharacter::SetActorToSceneCapture(TArray<TObjectPtr<AActor>>& ShowOnlyActors, TArray<TObjectPtr<AActor>>& HiddenActors)
{
	TArray<TObjectPtr<AActor>> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (TObjectPtr<AActor> EachActor : AttachedActors)
	{
		ShowOnlyActors.Add(EachActor);
	}
	ShowOnlyActors.Add(this);
	
	if (const UProceduralPyramidRTSComponent* PyramidRTSComponent = GetComponentByClass<UProceduralPyramidRTSComponent>())
	{
		PyramidRTSComponent->GetProceduralMesh()->SetHiddenInSceneCapture(true);
	}
	
}

void ASCharacter::EndInteractUnitRTS()
{
	if (UProceduralPyramidRTSComponent* PyramidRTSComponent = GetComponentByClass<UProceduralPyramidRTSComponent>())
	{
		PyramidRTSComponent->DestroyPyramidMesh();
	}
	if (UComponentInteractActorInScreen* ComponentInteractActorInScreen = GetComponentByClass<UComponentInteractActorInScreen>())
	{
		ComponentInteractActorInScreen->CloseActiveWidget();
	}
}


void ASCharacter::StartInteractUnitRTS()
{
	if (UProceduralPyramidRTSComponent* PyramidRTSComponent = GetComponentByClass<UProceduralPyramidRTSComponent>())
	{
		PyramidRTSComponent->CreatePyramidMesh();
	}
	if (UComponentInteractActorInScreen* ComponentInteractActorInScreen = GetComponentByClass<UComponentInteractActorInScreen>())
	{
		ComponentInteractActorInScreen->ShowActorOnWidget(this);
	}
}
