
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "RTSMoveSystem/Interfaces/ActorSceneRender.h"
#include "RTSMoveSystem/Interfaces/Unit.h"
#include "SCharacter.generated.h"

class UPostProcessComponent;

UCLASS()
class RTSMOVESYSTEM_API ASCharacter : public ACharacter, public IUnit, public IActorSceneRender
{
	GENERATED_BODY()

public:
	ASCharacter();

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Jump() override;
	
	virtual FTransform CalculateCameraSceneTransform(float DistanceFrom) const override;
	virtual USceneComponent* GetSceneRootComponent() const override;
	virtual void PrepareSelfForSceneRender() override;
	virtual void DisablingCollisions() override;
	virtual void DisablingGameLogic() override;
	virtual void SetSelfVisibleInSceneCaptureOnly() override;

	virtual void EndInteractUnitRTS() override;
	virtual void StartInteractUnitRTS() override;

	virtual void SetActorToSceneCapture(TArray<TObjectPtr<AActor>>& ShowOnlyActors,
		TArray<TObjectPtr<AActor>>& HiddenActors) override;
};
