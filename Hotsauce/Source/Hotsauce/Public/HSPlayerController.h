// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Active)
    bool IsPrimaryActive;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Active)
    float PrimaryCycleTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Active)
    float PrimaryDurationTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Active)
    bool IsSecondaryActive;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Active)
    float SecondaryCycleTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Active)
    float SecondaryDurationTime;

protected:    
	
    virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

    void OnPrimaryPressed();
    void OnPrimaryReleased();
    
    void OnSecondaryPressed();
    void OnSecondaryReleased();
};


