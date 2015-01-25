// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
//#include "GameFramework/AIController.h"
#include "HSPlayerCharacter.h"
#include "HSPlayerController.generated.h"

UCLASS()
class AHSPlayerController : public APlayerController
{
	GENERATED_BODY()

    virtual void BeginPlay() override;

public:
    AHSPlayerController(const FObjectInitializer& ObjectInitializer);

    float PanSpeed;
    bool IsZoomedIn;
    
protected:
	uint32 bMoveToMouseCursor : 1;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

    UFUNCTION(Reliable, Server, WithValidation)
    void SetDestination(const FVector aDestination);
    
	/** Input handlers for SetDestination action. */
    void OnSetDestinationPressed();
	void OnSetDestinationReleased();

    void MoveCameraHorizontalRate(float Rate);
    void MoveCameraVerticalRate(float Rate);
    void ResetCamera();
    void ToggleZoom();
};


