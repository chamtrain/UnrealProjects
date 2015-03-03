// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSPlayerController.h"
#include "HSPlayerCharacter.h"
#include "AI/Navigation/NavigationSystem.h"

AHSPlayerController::AHSPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
    PrimaryCycleTime = 0.2f;
    PrimaryDurationTime = 0.0f;
    SecondaryCycleTime = 5.0f;
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
    
    if (IsPrimaryActive)
    {
        PrimaryDurationTime += DeltaTime;
        if (PrimaryDurationTime > PrimaryCycleTime)
        {
            Primary();
            PrimaryDurationTime = 0.0f;
        }
    }
}

void AHSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

    // Action Mapping
    
    InputComponent->BindAction("Primary", IE_Pressed, this, &AHSPlayerController::OnPrimaryPressed);
    InputComponent->BindAction("Primary", IE_Released, this, &AHSPlayerController::OnPrimaryReleased);
    
    /*
    InputComponent->BindAction("Secondary", IE_Pressed, this, &AHSPlayerController::OnDefendPressed);
    InputComponent->BindAction("Secondary", IE_Released, this, &AHSPlayerController::OnDefendReleased);

    // Axis Mapping
    InputComponent->BindAxis("HorizontalMovement", this, &AHSPlayerController::HorizontalMovementRate);
    InputComponent->BindAxis("VerticalMovement", this, &AHSPlayerController::VerticalMovementRate);
    */
}

void AHSPlayerController::OnPrimaryPressed()
{
    IsPrimaryActive = true;
    OnPrimaryEvent();
    AHSPlayerCharacter* pc = (AHSPlayerCharacter*)GetCharacter();
    if (pc)
    {
        pc->EventSelectTarget();
    }
}

void AHSPlayerController::OnPrimaryReleased()
{
    IsPrimaryActive = false;
    PrimaryDurationTime = 10.0f;
}

void AHSPlayerController::OnSecondaryPressed()
{

}

void AHSPlayerController::OnSecondaryReleased()
{

}

void AHSPlayerController::HorizontalMovementRate(float Rate)
{
    
}

void AHSPlayerController::VerticalMovementRate(float Rate)
{

}

bool AHSPlayerController::Primary_Validate()
{
    return true;
}

void AHSPlayerController::Primary_Implementation()
{
    AHSPlayerCharacter* const PlayerCharacter = (AHSPlayerCharacter*)GetPawn();
    if (PlayerCharacter) {
        PlayerCharacter->OnFire();
    }
}