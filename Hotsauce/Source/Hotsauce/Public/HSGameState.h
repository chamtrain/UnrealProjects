// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "HSPlayerState.h"
#include "HSGameState.generated.h"

/** ranked PlayerState map, created from the GameState */
typedef TMap<int32, TWeakObjectPtr<AHSPlayerState> > RankedPlayerMap;

UCLASS()
class HOTSAUCE_API AHSGameState : public AGameState
{
	GENERATED_BODY()
	
public:
    AHSGameState(const FObjectInitializer& ObjectInitializer);

    /** number of teams in current game (doesn't deprecate when no players are left in a team) */
    UPROPERTY(Transient, Replicated)
    int32 NumTeams;

    /** accumulated score per team */
    UPROPERTY(Transient, Replicated)
    TArray<int32> TeamScores;

    /** time left for warmup / match */
    UPROPERTY(Transient, Replicated)
    int32 RemainingTime;

    /** is timer paused? */
    UPROPERTY(Transient, Replicated)
    bool bTimerPaused;

    /** gets ranked PlayerState map for specific team */
    void GetRankedMap(int32 TeamIndex, RankedPlayerMap& OutRankedMap) const;

    void RequestFinishAndExitToMainMenu();
};
