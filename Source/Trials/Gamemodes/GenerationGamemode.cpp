// Fill out your copyright notice in the Description page of Project Settings.

#include "Trials/Noise/PerlinNoiseLib.h"
#include "GenerationGamemode.h"

void AGenerationGamemode::BeginPlay()
{
	Super::BeginPlay();

    float scale = 0.05;
    int w = 100;
    int h = 100;
    float blockSize = 100.0f;
    float minHeight = 0.0f;
    float maxHeight = 300.0f;
    PerlinNoise perlinInstance;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double heightValue = perlinInstance.noise(x * scale, y * scale);
            float mappedHeight = minHeight + ((heightValue + 1) / 2) * (maxHeight - minHeight);
            FVector spawnLocation(x * blockSize, y * blockSize, mappedHeight);
            FRotator spawnRotation(0.0f, 0.0f, 0.0f);
            if (VoxelBlueprint)
            {
                GetWorld()->SpawnActor<AActor>(VoxelBlueprint, spawnLocation, spawnRotation);
            }
            UE_LOG(LogTemp, Warning, TEXT("Spawned at (%f, %f, %f) with height value: %f"), spawnLocation.X, spawnLocation.Y, spawnLocation.Z, heightValue);
        }
    }
}