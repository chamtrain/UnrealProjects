// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSPlayerController.h"
#include "HSProxyCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
    PanSpeed = 5.f;
    IsZoomedIn = true;
}

void AHSPlayerController::BeginPlay()
{
}

void AHSPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AHSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

    InputComponent->BindAction("SetDestination", IE_Pressed, this, &AHSPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction("SetDestination", IE_Released, this, &AHSPlayerController::OnSetDestinationReleased);
    InputComponent->BindAction("ResetCamera", IE_Pressed, this, &AHSPlayerController::ToggleZoom);

    InputComponent->BindAxis("CameraHorizontal", this, &AHSPlayerController::MoveCameraHorizontalRate);
    InputComponent->BindAxis("CameraVertical", this, &AHSPlayerController::MoveCameraVerticalRate);
}

void AHSPlayerController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
        SetDestination(Hit.ImpactPoint);
	}
}

void AHSPlayerController::OnSetDestinationPressed()
{
	bMoveToMouseCursor = true;
}

void AHSPlayerController::OnSetDestinationReleased()
{
	bMoveToMouseCursor = false;
}

void AHSPlayerController::MoveCameraHorizontalRate(float Rate)
{
    if (abs(Rate) < .5f)
    {
        return;
    }
    AHSProxyCharacter* const PlayerCharacter = (AHSProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(PlayerCharacter->CameraTargetOffset + Rate*PanSpeed*FVector(0.0f, 10.0f, 0.0f));
    }
}

void AHSPlayerController::MoveCameraVerticalRate(float Rate)
{
    if (abs(Rate) < .5f)
    {
        return;
    }
    AHSProxyCharacter* const PlayerCharacter = (AHSProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(PlayerCharacter->CameraTargetOffset + Rate*PanSpeed*FVector(10.0f, 0.0f, 0.0f));
    }
}

void AHSPlayerController::ResetCamera()
{
    AHSProxyCharacter* const PlayerCharacter = (AHSProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        PlayerCharacter->SetCamreaOffset(FVector(0.0f, 0.0f, 0.0f));
    }
}

void AHSPlayerController::ToggleZoom()
{
    AHSProxyCharacter* const PlayerCharacter = (AHSProxyCharacter*)GetPawn();
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

bool AHSPlayerController::SetDestination_Validate(const FVector DestLocation)
{
    return true;
}

void AHSPlayerController::SetDestination_Implementation(const FVector DestLocation)
{
    AHSProxyCharacter* const PlayerCharacter = (AHSProxyCharacter*)GetPawn();
    if (PlayerCharacter)
    {
        float const Distance = FVector::Dist(DestLocation, PlayerCharacter->GetActorLocation());

        if (Distance > 120.0f)
        {
            PlayerCharacter->MoveToLocation(this, DestLocation);
        }
    }
}