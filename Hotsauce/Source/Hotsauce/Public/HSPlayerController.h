// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "HSPlayerCharacter.h"
#include "HSPlayerController.generated.h"

UCLASS()
class AHSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

    AHSPlayerController(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mouse)
    FVector MouseHitLocation;

    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnPrimaryEvent();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Events")
    void OnSecondaryEvent();

protected:
    float PrimaryCycleTime;
    float PrimaryDurationTime;
    float SecondaryCycleTime;
    float SecondaryDurationTime;
    bool IsPrimaryActive;
    bool IsSecondaryActive;
	
    virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

    void OnPrimaryPressed();
    void OnPrimaryReleased();
    

    void OnSecondaryPressed();
    void OnSecondaryReleased();
    

    void HorizontalMovementRate(float Rate);
    void VerticalMovementRate(float Rate);

    UFUNCTION(Reliable, Server, WithValidation)
    void Primary();

};


