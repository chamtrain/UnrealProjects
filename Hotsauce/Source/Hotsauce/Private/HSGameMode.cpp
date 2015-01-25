// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Hotsauce.h"
#include "HSGameMode.h"
#include "HSPlayerController.h"
#include "HSProxyCharacter.h"

AHSGameMode::AHSGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = AHSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/MyCharacter"));
    //Blueprint'/Game/Blueprints/MyCharacter.MyCharacter'
    //Blueprint'/Game/Blueprints/Bots/PlayerControlledBot/PCAICharacter.PCAICharacter'
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Bots/PlayerControlledBot/PCAICharacter"));
    /*
    if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    */
    DefaultPawnClass = AHSProxyCharacter::StaticClass();
}