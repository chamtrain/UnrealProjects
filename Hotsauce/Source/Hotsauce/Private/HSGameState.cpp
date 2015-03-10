// Fill out your copyright notice in the Description page of Project Settings.

#include "Hotsauce.h"
#include "HSGameState.h"
#include "HSGameMode.h"
#include "UnrealNetwork.h"
#include "Engine.h"

AHSGameState::AHSGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    NumTeams = 0;
    RemainingTime = 0;
    bTimerPaused = false;
}

void AHSGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSGameState, NumTeams);
    DOREPLIFETIME(AHSGameState, RemainingTime);
    DOREPLIFETIME(AHSGameState, bTimerPaused);
    DOREPLIFETIME(AHSGameState, TeamScores);
}

void AHSGameState::GetRankedMap(int32 TeamIndex, RankedPlayerMap& OutRankedMap) const
{
    OutRankedMap.Empty();

    //first, we need to go over all the PlayerStates, grab their score, and rank them
    TMultiMap<int32, AHSPlayerState*> SortedMap;
    for (int32 i = 0; i < PlayerArray.Num(); ++i)
    {
        int32 Score = 0;
        AHSPlayerState* CurPlayerState = Cast<AHSPlayerState>(PlayerArray[i]);
        if (CurPlayerState && (CurPlayerState->GetTeamNum() == TeamIndex))
        {
            SortedMap.Add(FMath::TruncToInt(CurPlayerState->Score), CurPlayerState);
        }
    }

    //sort by the keys
    SortedMap.KeySort(TGreater<int32>());

    //now, add them back to the ranked map
    OutRankedMap.Empty();

    int32 Rank = 0;
    for (TMultiMap<int32, AHSPlayerState*>::TIterator It(SortedMap); It; ++It)
    {
        OutRankedMap.Add(Rank++, It.Value());
    }

}


void AHSGameState::RequestFinishAndExitToMainMenu()
{
    /*
    if (AuthorityGameMode)
    {
        // we are server, tell the gamemode
        AHSGameMode* const GameMode = Cast<AHSGameMode>(AuthorityGameMode);
        if (GameMode)
        {
            GameMode->RequestFinishAndExitToMainMenu();
        }
    }
    else
    {
        
        // we are client, handle our own business
        UShooterGameInstance* GI = Cast<UShooterGameInstance>(GetGameInstance());
        if (GI)
        {
            GI->RemoveSplitScreenPlayers();
        }
        

        AHSPlayerController* const PrimaryPC = Cast<AHSPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
        if (PrimaryPC)
        {
            check(PrimaryPC->GetNetMode() == ENetMode::NM_Client);
            PrimaryPC->HandleReturnToMainMenu();
        }
    }
    */
}
