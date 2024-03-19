
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSMoveSystem/Global/Macros/ControllersMacros.h"
#include "RTSMoveSystem/Interfaces/Unit.h"
#include "SPlayerController.generated.h"


class IUnit;
class UInputAction;
class UInputMappingContext;

UCLASS()
class RTSMOVESYSTEM_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeftMouseClickDelegate, bool, bClicked);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

public:
	virtual void OnPossess(APawn* InPawn) override;

	void SetMouseCursorRTS(bool CursorVisible);

	FOnLeftMouseClickDelegate OnLeftMouseClick;

protected:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	void HandleLeftMouseClick() const;

	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdatePawnInputContext(APawn* OldPawnInner, APawn* NewPawnInner);
	
	PARAMETERIZED_ACTION_TO_UNIT_INTERFACE(Move, const FInputActionValue&)
	PARAMETERIZED_ACTION_TO_UNIT_INTERFACE(Look, const FInputActionValue&)
	SIMPLE_ACTION_TO_UNIT_INTERFACE(Jump)

	TScriptInterface<IUnit> IUnitInterface;

};
