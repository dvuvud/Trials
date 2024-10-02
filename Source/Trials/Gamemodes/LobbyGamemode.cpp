// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGamemode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGamemode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    NumberOfPlayers = GameState.Get()->PlayerArray.Num();
    /*
    if (NumberOfPlayers == 2)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            bUseSeamlessTravel = true;
            World->ServerTravel(FString("/Game/Maps/Eureka?listen"));
        }
    }

    */
}

void ALobbyGamemode::MoveToEureka()
{
    UWorld* World = GetWorld();
    if (World)
    {
        bUseSeamlessTravel = true;
        World->ServerTravel(FString("/Game/Maps/Opal?listen"));
    }
}
