
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "Unit.generated.h"

UINTERFACE(MinimalAPI)
class UUnit : public UInterface
{
	GENERATED_BODY()
};


class RTSMOVESYSTEM_API IUnit
{
	GENERATED_BODY()

public:
	virtual void RemoveInputMappingContext() {}
	virtual void AddInputMappingContext() {}

	virtual void Move(const FInputActionValue& Value) {}
	virtual void Look(const FInputActionValue& Value) {}
	virtual void Jump() {}

	virtual void DisablingCollisions() {}
	virtual void DisablingGameLogic() {}

	virtual void EndInteractUnitRTS() {}
	virtual void StartInteractUnitRTS() {}

};
