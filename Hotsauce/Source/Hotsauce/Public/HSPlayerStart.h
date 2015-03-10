// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "HSPlayerStart.generated.h"

UCLASS()
class HOTSAUCE_API AHSPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:

    AHSPlayerStart(const FObjectInitializer& ObjectInitializer);
	
    /** Which team can start at this point */
    UPROPERTY(EditInstanceOnly, Category = Team)
    int32 SpawnTeam;

    /** Whether players can start at this point */
    UPROPERTY(EditInstanceOnly, Category = Team)
    uint32 bForPlayers : 1;

    /** Whether bots can start at this point */
    UPROPERTY(EditInstanceOnly, Category = Team)
    uint32 bForBots : 1;
	
};
