// Fill out your copyright notice in the Description page of Project Settings.
#include "GenerationGamemode.h"
#include "Trials/Noise/PerlinNoiseLib.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Trials/Blocks/Voxel.h"

void AGenerationGamemode::BeginPlay()
{
	Super::BeginPlay();
    VoxelActor = GetWorld()->SpawnActor<AVoxel>(VoxelActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
    if (VoxelActor)
    {
        VoxelActor->GetISM()->RegisterComponent();
    }
    PerlinNoise Perlin;
    GenerateVoxelTerrain(Perlin, FVector::ZeroVector);
}

void AGenerationGamemode::GenerateVoxelTerrain(PerlinNoise& NoiseGenerator, FVector Origin)
{
    // Loop over a grid to place voxel blocks
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            // Calculate world position of this voxel
            float WorldX = Origin.X + x * VOXEL_SIZE;
            float WorldY = Origin.Y + y * VOXEL_SIZE;

            // Sample Perlin noise to get the height value (in Z)
            double NoiseValue = NoiseGenerator.noise(WorldX * FREQUENCY / UNIT_SIZE, WorldY * FREQUENCY / UNIT_SIZE) * AMPLITUDE + 
                NoiseGenerator.noise(WorldX * FREQUENCY / UNIT_SIZE, WorldY * FREQUENCY * 2 / UNIT_SIZE) * AMPLITUDE / 2+ 
                NoiseGenerator.noise(WorldX * FREQUENCY / UNIT_SIZE, WorldY * FREQUENCY * 4/ UNIT_SIZE) * AMPLITUDE / 4;
            // Scale the noise value to fit within a range of Z heights
            float Height = FMath::Clamp((NoiseValue), 0.0f, 20.0f * VOXEL_SIZE); // Adjust height range
            int NumVoxelsHeight = FMath::FloorToInt(Height / VOXEL_SIZE); // Calculate the number of voxel layers

            // Spawn voxel blocks in Z layers (stack them)
            for (int z = 0; z < NumVoxelsHeight + 1; ++z) {
                float WorldZ = Origin.Z + z * VOXEL_SIZE;

                FTransform SpawnTransform;
                SpawnTransform.SetLocation(FVector(WorldX, WorldY, WorldZ));
                // Spawn a voxel block at (WorldX, WorldY, WorldZ)
                if (VoxelActor && VoxelActor->GetISM())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Noise Value: %f"), NoiseValue);
                    VoxelActor->GetISM()->AddInstance(SpawnTransform);
                }
            }
        }
    }
    int32 InstanceCount = VoxelActor->GetISM()->GetInstanceCount();
    UE_LOG(LogTemp, Warning, TEXT("Total Instances Added: %d"), InstanceCount);
}
