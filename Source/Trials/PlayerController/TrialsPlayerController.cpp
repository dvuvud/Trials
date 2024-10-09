// Fill out your copyright notice in the Description page of Project Settings.


#include "TrialsPlayerController.h"
#include "Trials/Gamemodes/GenerationGamemode.h"
#include "Kismet/GameplayStatics.h"

void ATrialsPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (IsLocalPlayerController() && GetPawn())
    {
        PlayerLocation = GetPawn()->GetActorLocation();

        int32 PlayerChunkX = FMath::FloorToInt(PlayerLocation.X / (VOXELSIZE * CHUNK_SIZE));
        int32 PlayerChunkY = FMath::FloorToInt(PlayerLocation.Y / (VOXELSIZE * CHUNK_SIZE));
        for (int32 OffsetX = PlayerChunkX - LOAD_DISTANCE; OffsetX <= PlayerChunkX + LOAD_DISTANCE; OffsetX++)
        {
            for (int32 OffsetY = PlayerChunkY - LOAD_DISTANCE; OffsetY <= PlayerChunkY + LOAD_DISTANCE; OffsetY++)
            {
                // Calculate the chunk coordinates
                FIntVector ChunkCoord(OffsetX, OffsetY, 0);
                ChunkCoord *= CHUNK_SIZE * VOXELSIZE;

                // Check if the chunk is already loaded
                if (!LoadedChunks.Contains(ChunkCoord))
                {
                    // If the chunk is not loaded, request it from the game mode (or generation system)
                    FVector2D ChunkOrigin = FVector2D(ChunkCoord.X, ChunkCoord.Y);
                    ServerLoadChunks(ChunkOrigin);
                    UE_LOG(LogTemp, Warning, TEXT("ChunkOrigin Values: %f : %f"), ChunkOrigin.X, ChunkOrigin.Y);
                    // Mark this chunk as loaded
                    LoadedChunks.Add(ChunkCoord);
                }
            }
        }
    }
}

void ATrialsPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		Gamemode = Cast<AGenerationGamemode>(UGameplayStatics::GetGameMode(this));
	}
}

void ATrialsPlayerController::ServerLoadChunks_Implementation(FVector2D ChunkOrigin)
{
	if (Gamemode)
	{
		Gamemode->GenerateVoxelTerrain(ChunkOrigin);
	}
}
