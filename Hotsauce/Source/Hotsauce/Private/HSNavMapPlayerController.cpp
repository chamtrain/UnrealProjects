// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSNavMapPlayerController.h"
#include "HSNavMapProxyCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AHSNavMapPlayerController::AHSNavMapPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
    PanSpeed = 5.f;
    IsZoomedIn = true;
}

void AHSNavMapPlayerController::BeginPlay()
{
}

void AHSNavMapPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AHSNavMapPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

    // Action Mapping SetDestination
    InputComponent->BindAction("SetDestination", IE_Pressed, this, &AHSNavMapPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction("SetDestination", IE_Released, this, &AHSNavMapPlayerController::OnSetDestinationReleased);
    InputComponent->BindAction("ResetCamera", IE_Pressed, this, &AHSNavMapPlayerController::ToggleZoom);

    // Axis Mapping CameraHorizontal, CameraVertical
    InputComponent->BindAxis("CameraHorizontal", this, &AHSNavMapPlayerController::MoveCameraHorizontalRate);
    InputComponent->BindAxis("CameraVertical", this, &AHSNavMapPlayerController::MoveCameraVerticalRate);
}

void AHSNavMapPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
        SetDestination(Hit.ImpactPoint);
	}
}

void AHSNavMapPlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void AHSNavMapPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void AHSNavMapPlayerController::MoveCameraHorizontalRate(float Rate)
{
    if (abs(Rate) < .5f)
    {
        return;
    }
    AHSNavMapProxyCharacter* const PlayerCharacter = (AHSNavMapProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(PlayerCharacter->CameraTargetOffset + Rate*PanSpeed*FVector(0.0f, 10.0f, 0.0f));
    }
}

void AHSNavMapPlayerController::MoveCameraVerticalRate(float Rate)
{
    if (abs(Rate) < .5f)
    {
        return;
    }
    AHSNavMapProxyCharacter* const PlayerCharacter = (AHSNavMapProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(PlayerCharacter->CameraTargetOffset + Rate*PanSpeed*FVector(10.0f, 0.0f, 0.0f));
    }
}

void AHSNavMapPlayerController::ResetCamera()
{
    AHSNavMapProxyCharacter* const PlayerCharacter = (AHSNavMapProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(FVector(0.0f, 0.0f, 0.0f));
    }
}

void AHSNavMapPlayerController::ToggleZoom()
{
    AHSNavMapProxyCharacter* const PlayerCharacter = (AHSNavMapProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        if (IsZoomedIn)
        {
            PlayerCharacter->SetCamreaHeight(2500.f);
            IsZoomedIn = false;
        }
        else
        {
            PlayerCharacter->SetCamreaHeight(800.f);
            ResetCamera();
            IsZoomedIn = true;
        }
    }
}

bool AHSNavMapPlayerController::SetDestination_Validate(const FVector DestLocation)
{
    return true;
}

void AHSNavMapPlayerController::SetDestination_Implementation(const FVector DestLocation)
{
    AHSNavMapProxyCharacter* const PlayerCharacter = (AHSNavMapProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        float const Distance = FVector::Dist(DestLocation, PlayerCharacter->GetActorLocation());

        if (Distance > 120.0f)
        {
            PlayerCharacter->MoveToLocation(this, DestLocation);
        }
    }
}