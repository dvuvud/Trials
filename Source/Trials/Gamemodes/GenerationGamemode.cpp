// Fill out your copyright notice in the Description page of Project Settings.
#include "GenerationGamemode.h"
#include "Trials/Noise/PerlinNoiseLib.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Trials/Blocks/Voxel.h"

void AGenerationGamemode::BeginPlay()
{
	Super::BeginPlay();
    ChunkVoxelActor = GetWorld()->SpawnActor<AVoxel>(VoxelActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
    if (ChunkVoxelActor && ChunkVoxelActor->GetISM())
    {
        ChunkVoxelActor->GetISM()->RegisterComponent();
    }
}

void AGenerationGamemode::GenerateVoxelTerrain(FVector2D Origin)
{
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            // Calculate world position of this voxel
            float WorldX = Origin.X + x * VOXEL_SIZE;
            float WorldY = Origin.Y + y * VOXEL_SIZE;

            double NoiseValue = PerlinNoise::noise(WorldX * FREQUENCY / UNIT_SIZE, WorldY * FREQUENCY / UNIT_SIZE) * AMPLITUDE +
                PerlinNoise::noise(WorldX * FREQUENCY * 2 / UNIT_SIZE, WorldY * FREQUENCY * 2 / UNIT_SIZE) * AMPLITUDE / 2+
                PerlinNoise::noise(WorldX * FREQUENCY * 4 / UNIT_SIZE, WorldY * FREQUENCY * 4/ UNIT_SIZE) * AMPLITUDE / 4;

            float Height = FMath::Clamp((NoiseValue), -10.f * VOXEL_SIZE, 20.0f * VOXEL_SIZE);
            int NumVoxelsHeight = FMath::FloorToInt(Height / VOXEL_SIZE);

            for (int z = -10; z < NumVoxelsHeight + 1; ++z) {
                float WorldZ = z * VOXEL_SIZE;

                FTransform SpawnTransform;
                SpawnTransform.SetLocation(FVector(WorldX, WorldY, WorldZ));

                if (ChunkVoxelActor && ChunkVoxelActor->GetISM())
                {
                    ChunkVoxelActor->GetISM()->AddInstance(SpawnTransform);
                }
            }
        }
    }
}
