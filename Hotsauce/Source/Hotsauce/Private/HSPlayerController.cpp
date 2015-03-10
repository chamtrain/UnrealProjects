// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSPlayerController.h"
#include "HSCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryCycleTime = 0.2f;
    PrimaryDurationTime = 0.0f;
    SecondaryCycleTime = .2f;
    SecondaryDurationTime = 0.0f;
    IsPrimaryActive = false;
    IsSecondaryActive = false;

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableTouchEvents = true;
    bEnableMouseOverEvents = true;
    bEnableTouchOverEvents = true;
}

void AHSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        MouseHitLocation = Hit.ImpactPoint;
    }
}

void AHSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

    // Action Mapping
    
    InputComponent->BindAction("Primary", IE_Pressed, this, &AHSPlayerController::OnPrimaryPressed);
    InputComponent->BindAction("Primary", IE_Released, this, &AHSPlayerController::OnPrimaryReleased);
    
    InputComponent->BindAction("Secondary", IE_Pressed, this, &AHSPlayerController::OnSecondaryPressed);
    InputComponent->BindAction("Secondary", IE_Released, this, &AHSPlayerController::OnSecondaryReleased);
    
    /*
    // Axis Mapping
    InputComponent->BindAxis("HorizontalMovement", this, &AHSPlayerController::HorizontalMovementRate);
    InputComponent->BindAxis("VerticalMovement", this, &AHSPlayerController::VerticalMovementRate);
    */
}

void AHSPlayerController::OnPrimaryPressed()
{
    IsPrimaryActive = true;
}

void AHSPlayerController::OnPrimaryReleased()
{
    IsPrimaryActive = false;
}

void AHSPlayerController::OnSecondaryPressed()
{
    IsSecondaryActive = true;
}

void AHSPlayerController::OnSecondaryReleased()
{
    IsSecondaryActive = false;
}