// Fill out your copyright notice in the Description page of Project Settings.

#include "Hotsauce.h"
#include "HSPlayerState.h"
#include "UnrealNetwork.h"
#include "Engine.h"

AHSPlayerState::AHSPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    TeamNumber = 0;
    NumKills = 0;
    NumDeaths = 0;
    NumBulletsFired = 0;
    NumRocketsFired = 0;
    bQuitter = false;
}

void AHSPlayerState::Reset()
{
    Super::Reset();

    //PlayerStates persist across seamless travel.  Keep the same teams as previous match.
    //SetTeamNum(0);
    NumKills = 0;
    NumDeaths = 0;
    NumBulletsFired = 0;
    NumRocketsFired = 0;
    bQuitter = false;
}

void AHSPlayerState::UnregisterPlayerWithSession()
{
    if (!bFromPreviousLevel)
    {
        Super::UnregisterPlayerWithSession();
    }
}

void AHSPlayerState::ClientInitialize(class AController* InController)
{
    Super::ClientInitialize(InController);

    UpdateTeamColors();
}

void AHSPlayerState::SetTeamNum(int32 NewTeamNumber)
{
    TeamNumber = NewTeamNumber;

    UpdateTeamColors();
}

void AHSPlayerState::OnRep_TeamColor()
{
    UpdateTeamColors();
}

void AHSPlayerState::AddBulletsFired(int32 NumBullets)
{
    NumBulletsFired += NumBullets;
}

void AHSPlayerState::AddRocketsFired(int32 NumRockets)
{
    NumRocketsFired += NumRockets;
}

void AHSPlayerState::SetQuitter(bool bInQuitter)
{
    bQuitter = bInQuitter;
}

void AHSPlayerState::CopyProperties(class APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    AHSPlayerState* ShooterPlayer = Cast<AHSPlayerState>(PlayerState);
    if (ShooterPlayer)
    {
        ShooterPlayer->TeamNumber = TeamNumber;
    }
}

void AHSPlayerState::UpdateTeamColors()
{
    AController* OwnerController = Cast<AController>(GetOwner());
    if (OwnerController != NULL)
    {
        /*
        AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerController->GetCharacter());
        if (ShooterCharacter != NULL)
        {
            ShooterCharacter->UpdateTeamColorsAllMIDs();
        }
        */
    }
}

int32 AHSPlayerState::GetTeamNum() const
{
    return TeamNumber;
}

int32 AHSPlayerState::GetKills() const
{
    return NumKills;
}

int32 AHSPlayerState::GetDeaths() const
{
    return NumDeaths;
}

float AHSPlayerState::GetScore() const
{
    return Score;
}

int32 AHSPlayerState::GetNumBulletsFired() const
{
    return NumBulletsFired;
}

int32 AHSPlayerState::GetNumRocketsFired() const
{
    return NumRocketsFired;
}

bool AHSPlayerState::IsQuitter() const
{
    return bQuitter;
}

void AHSPlayerState::ScoreKill(AHSPlayerState* Victim, int32 Points)
{
    NumKills++;
    ScorePoints(Points);
}

void AHSPlayerState::ScoreDeath(AHSPlayerState* KilledBy, int32 Points)
{
    NumDeaths++;
    ScorePoints(Points);
}

void AHSPlayerState::ScorePoints(int32 Points)
{
    /*
    AShooterGameState* const MyGameState = Cast<AShooterGameState>(GetWorld()->GameState);
    if (MyGameState && TeamNumber >= 0)
    {
        if (TeamNumber >= MyGameState->TeamScores.Num())
        {
            MyGameState->TeamScores.AddZeroed(TeamNumber - MyGameState->TeamScores.Num() + 1);
        }

        MyGameState->TeamScores[TeamNumber] += Points;
    }

    Score += Points;
    */
}

void AHSPlayerState::InformAboutKill_Implementation(class AHSPlayerState* KillerPlayerState, const UDamageType* KillerDamageType, class AHSPlayerState* KilledPlayerState)
{
    //id can be null for bots
    if (KillerPlayerState->UniqueId.IsValid())
    {
        //search for the actual killer before calling OnKill()	
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            /*
            AShooterPlayerController* TestPC = Cast<AShooterPlayerController>(*It);
            if (TestPC && TestPC->IsLocalController())
            {
                // a local player might not have an ID if it was created with CreateDebugPlayer.
                ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(TestPC->Player);
                TSharedPtr<FUniqueNetId> LocalID = LocalPlayer->GetCachedUniqueNetId();
                if (LocalID.IsValid() && *LocalPlayer->GetCachedUniqueNetId() == *KillerPlayerState->UniqueId)
                {
                    TestPC->OnKill();
                }
            }
            */
        }
    }
}

void AHSPlayerState::BroadcastDeath_Implementation(class AHSPlayerState* KillerPlayerState, const UDamageType* KillerDamageType, class AHSPlayerState* KilledPlayerState)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        // all local players get death messages so they can update their huds.
        /*
        AShooterPlayerController* TestPC = Cast<AShooterPlayerController>(*It);
        if (TestPC && TestPC->IsLocalController())
        {
            TestPC->OnDeathMessage(KillerPlayerState, this, KillerDamageType);
        }
        */
    }
}

void AHSPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHSPlayerState, TeamNumber);
    DOREPLIFETIME(AHSPlayerState, NumKills);
    DOREPLIFETIME(AHSPlayerState, NumDeaths);
}

FString AHSPlayerState::GetShortPlayerName() const
{
    /*
    if (PlayerName.Len() > MAX_PLAYER_NAME_LENGTH)
    {
        return PlayerName.Left(MAX_PLAYER_NAME_LENGTH) + "...";
    }
    */
    return PlayerName;
}
