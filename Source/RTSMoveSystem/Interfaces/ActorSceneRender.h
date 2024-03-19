
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorSceneRender.generated.h"


UINTERFACE(MinimalAPI)
class UActorSceneRender : public UInterface
{
	GENERATED_BODY()
};


class RTSMOVESYSTEM_API IActorSceneRender
{
	GENERATED_BODY()
	
public:
	virtual FTransform CalculateCameraSceneTransform(float DistanceOffset) const { return FTransform(); }
	virtual USceneComponent* GetSceneRootComponent() const { return nullptr; }
	virtual void DisablingCollisionsAndLogic() {}
	
	virtual void PrepareSelfForSceneRender() {}
	virtual void SetSelfVisibleInSceneCaptureOnly() {}
	
	virtual void SetActorToSceneCapture(TArray<TObjectPtr<AActor>>& ShowOnlyActors, TArray<TObjectPtr<AActor>>& HiddenActors) {}
};
